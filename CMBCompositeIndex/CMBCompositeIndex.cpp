//---------------------------------------------------------------------------
// CMBCompositeIndex indicator
//---------------------------------------------------------------------------

#include <windows.h>
#include "IndicatorInterfaceUnit.h"

#include <iostream>
#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>

enum TPriceType { pt_Close, pt_Open, pt_High, pt_Low, pt_HL2, pt_HLC3, pt_HLCC4 };

int InputRSISlowPeriod = 14;
int InputRSIFastPeriod = 3;
int InputMomentumPeriod = 9;
int InputMAPeriod1 = 3;
int InputMAPeriod2 = 13;
int InputMAPeriod3 = 33;
TPriceType InputPriceMode = pt_Close;

// Buffers
TIndexBuffer CompositeBuffer, FastMABuffer, SlowMABuffer;

//---------------------------------------------------------------------------
// Initialize indicator
//---------------------------------------------------------------------------
EXPORT void Init()
{
    // define properties
    char indicatorName[] = "CMB Composite Index";
    char seperatorName[] = "Common";

    IndicatorShortName(indicatorName);
    SetOutputWindow(ow_SeparateWindow);

    // register options
    AddSeparator(seperatorName);

    char InputMAPeriod1Label[] = "Input MA Period 1";
    char InputMAPeriod2Label[] = "Input MA Period 2";
    char InputMAPeriod3Label[] = "Input MA Period 3";

    RegOption(InputMAPeriod1Label, ot_Integer, &InputMAPeriod1);
    SetOptionRange(InputMAPeriod1Label, 1, INT32_MAX);
    InputMAPeriod1 = 3;

    RegOption(InputMAPeriod2Label, ot_Integer, &InputMAPeriod2);
    SetOptionRange(InputMAPeriod2Label, 1, INT32_MAX);
    InputMAPeriod2 = 13;

    RegOption(InputMAPeriod3Label, ot_Integer, &InputMAPeriod3);
    SetOptionRange(InputMAPeriod3Label, 1, INT32_MAX);
    InputMAPeriod1 = 33;

    // create buffers
    CompositeBuffer = CreateIndexBuffer();
    FastMABuffer = CreateIndexBuffer();
    SlowMABuffer = CreateIndexBuffer();
    
    IndicatorBuffers(3);
    SetIndexBuffer(0, CompositeBuffer);
    SetIndexBuffer(1, FastMABuffer);
    SetIndexBuffer(2, SlowMABuffer);
    
    char CompositeLabel[] = "Composite index";
    char FastMALabel[] = "Composite index Fast SMA";
    char SlowMALabel[] = "Composite index Slow SMA";
    SetIndexStyle(0, ds_Line, psSolid, 1, clDarkGray);
    SetIndexLabel(0, CompositeLabel);
    SetIndexStyle(1, ds_Line, psSolid, 1, clLimeGreen);
    SetIndexLabel(1, FastMALabel);
    SetIndexStyle(2, ds_Line, psSolid, 1, clOrangeRed);
    SetIndexLabel(2, SlowMALabel);
    //SetFixedMinMaxValues(0, 100);
}

EXPORT void OnParamsChange()
{
    
}

double GetEMA(const std::vector<double>& priceBars, int index, int shift, int period, double prev)
{
    int i;
    double k, sum, weight;

    // EMA
    // decay has been changed from 2.0 / (span + 1) -> 1.0 / (1.0 + com)
    // this makes it match MetaTrader's way of calculating RSI
    int com = period - 1;
    k = 1.0 / (com + 1);
    i = index + shift;
    if ((i > Bars() - 1) || (i < 0)) return 0;

    if (prev == 0)
        return priceBars[i];
    else
        return (prev + k * (priceBars[i] - prev));
}

double GetSMA(const std::vector<double>& priceBars, int index, int shift, int period)
{
    int i;
    double sum;

    // SMA
    if ((index + shift + period >= Bars()) || (index < 0)) return 0;

    sum = 0;
    for (i = index; i < index + period; i++) {
        sum += priceBars[i + shift];
    }

    return sum / period;
}

double GetPrice(int index, TPriceType PriceType)
{
    switch (PriceType)
    {
    case pt_Close: return Close(index);
    case pt_Open:  return Open(index);
    case pt_High:  return High(index);
    case pt_Low:   return Low(index);
    case pt_HL2:   return (High(index) + Low(index)) / 2;
    case pt_HLC3:  return (High(index) + Low(index) + Close(index)) / 3;
    case pt_HLCC4: return (High(index) + Low(index) + Close(index) * 2) / 4;
    }

    return 0;
}

std::vector<double> GetRSI(int total, int period, TPriceType ApplyTo) {
    // aggregate the collection
    std::vector<double> base;
    base.reserve(total);

    for (size_t i = 0; i < total; i++) {
        base.insert(base.begin(), GetPrice(i, ApplyTo));
    }

    std::adjacent_difference(base.begin(), base.end(), base.begin());
    base[0] = 0.0;

    std::vector<double> up, down;
    up.reserve(total);
    down.reserve(total);
    std::transform(base.cbegin(), base.cend(), std::back_inserter(up), [](const double value) { return value > 0 ? value : 0; });
    std::transform(base.cbegin(), base.cend(), std::back_inserter(down), [](const double value) { return value < 0 ? -1 * value : 0; });

    std::vector<double> target = { 0 };
    double maUpPrev = 0;
    double maDownPrev = 0;
    for (int i = 1; i < total; i++) {
        maUpPrev = GetEMA(up, i, 0, period, maUpPrev);
        maDownPrev = GetEMA(down, i, 0, period, maDownPrev);
        
        double maUp = maUpPrev;
        double maDown = maDownPrev;
        double RS = maUp / maDown;
        target.push_back(100 - (100 / (1 + RS)));
    }

    return target;
}

std::vector<double> _RSISlowCache;
std::vector<double> _RSIFastCache;
std::vector<double> _CompositeCache;
std::vector<double> _FastMACache;
std::vector<double> _SlowMACache;
std::string _cacheKey;

void PrecalculateIfChanged(std::string key) {
    if (_cacheKey == key) {
        return;
    }

    _cacheKey = key;
    // Print(std::string("CMB Composite Index: precalculating for key: ") + key);

    _RSISlowCache = GetRSI(Bars(), InputRSISlowPeriod, InputPriceMode);
    _RSIFastCache = GetRSI(Bars(), InputRSIFastPeriod, InputPriceMode);
    _CompositeCache.reserve(Bars());
    for (size_t i = 0; i < Bars(); i++) {
        double RSIDelta = i > InputMomentumPeriod ? _RSISlowCache[i] - _RSISlowCache[i - InputMomentumPeriod] : 0;
        double RSISMA = GetSMA(_RSIFastCache, i - InputMAPeriod1, 0, InputMAPeriod1);
        _CompositeCache[i] = RSIDelta + RSISMA;
    }

    _FastMACache.reserve(Bars());
    _SlowMACache.reserve(Bars());
    for (size_t i = 0; i < Bars(); i++) {
        _FastMACache[i] = GetSMA(_CompositeCache, i - InputMAPeriod2, 0, InputMAPeriod2);
        _SlowMACache[i] = GetSMA(_CompositeCache, i - InputMAPeriod3, 0, InputMAPeriod3);
    }
}

//---------------------------------------------------------------------------
// Calculate requested bar
//---------------------------------------------------------------------------
EXPORT void Calculate(int index)
{
    std::stringstream key; key << Bars() << "," << Timeframe() << "," << Symbol();
    PrecalculateIfChanged(key.str());

    // like MT4, ForexTester is indexing from most recent to oldest
    int i = Bars() - 1 - index;
    CompositeBuffer[index] = _CompositeCache[i];
    FastMABuffer[index] = _FastMACache[i];
    SlowMABuffer[index] = _SlowMACache[i];
}