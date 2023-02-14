//---------------------------------------------------------------------------
// Ishimoku indicator
//---------------------------------------------------------------------------

#include <windows.h>
#include "IndicatorInterfaceUnit.h"

#include <iostream>
#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>

enum TPriceType { pt_Close, pt_Open, pt_High, pt_Low, pt_HL2, pt_HLC3, pt_HLCC4 };
TPriceType InputPriceMode = pt_Close;

int PriceRSISlowPeriod;
int PriceRSIFastPeriod;
int CompositeRSIMomentumPeriod;
int CompositeRSISMAPeriod;
int CompositeFastSMAPeriod;
int CompositeSlowSMAPeriod;

// Buffers
TIndexBuffer Composite, FastMA, SlowMA;

//---------------------------------------------------------------------------
// Initialize indicator
//---------------------------------------------------------------------------
EXPORT void __stdcall Init()
{
    // define properties
    IndicatorShortName("CMB Composite Index");
    SetOutputWindow(ow_SeparateWindow);

    // register options
    AddSeparator("Common");

    RegOption("Price RSI Slow period", ot_Integer, &PriceRSISlowPeriod);
    SetOptionRange("Price RSI Slow period", 1, MAXINT32);
    PriceRSISlowPeriod = 14;

    RegOption("Price RSI Fast period", ot_Integer, &PriceRSIFastPeriod);
    SetOptionRange("Price RSI Fast period", 1, MAXINT32);
    PriceRSIFastPeriod = 3;

    RegOption("Composite RSI Momentum period", ot_Integer, &CompositeRSIMomentumPeriod);
    SetOptionRange("Composite RSI Momentum period", 1, MAXINT32);
    CompositeRSIMomentumPeriod = 9;

    RegOption("Composite RSI SMA period", ot_Integer, &CompositeRSISMAPeriod);
    SetOptionRange("Composite RSI SMA period", 1, MAXINT32);
    CompositeRSISMAPeriod = 3;

    RegOption("Fast SMA period", ot_Integer, &CompositeFastSMAPeriod);
    SetOptionRange("Fast SMA period", 1, MAXINT32);
    CompositeFastSMAPeriod = 13;

    RegOption("Slow SMA period", ot_Integer, &CompositeSlowSMAPeriod);
    SetOptionRange("Slow SMA period", 1, MAXINT32);
    CompositeSlowSMAPeriod = 33;

    // create buffers
    Composite = CreateIndexBuffer();
    FastMA = CreateIndexBuffer();
    SlowMA = CreateIndexBuffer();

    IndicatorBuffers(3);
    SetIndexBuffer(0, Composite);
    SetIndexBuffer(1, FastMA);
    SetIndexBuffer(2, SlowMA);

    SetIndexStyle(0, ds_Line, psSolid, 1, clDarkGray);
    SetIndexLabel(0, "Composite index");
    SetIndexStyle(1, ds_Line, psSolid, 1, clLimeGreen);
    SetIndexLabel(1, "Composite index Fast MA");
    SetIndexStyle(2, ds_Line, psSolid, 1, clOrangeRed);
    SetIndexLabel(2, "Composite index Slow MA");
}

EXPORT void __stdcall OnParamsChange()
{
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
    if ((i > priceBars.size() - 1) || (i < 0)) return 0;

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
    if ((index + shift + period >= priceBars.size()) || (index < 0)) return 0;

    sum = 0;
    for (i = index; i < index + period; i++) {
        sum += priceBars[i + shift];
    }

    return sum / period;
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
    //Print(std::string("CMB Composite Index: precalculating for key: ") + key);

    _RSISlowCache = GetRSI(Bars(), PriceRSISlowPeriod, InputPriceMode);
    _RSIFastCache = GetRSI(Bars(), PriceRSIFastPeriod, InputPriceMode);
    _CompositeCache.reserve(Bars());
    for (size_t i = 0; i < Bars(); i++) {
        double RSIDelta = i > CompositeRSIMomentumPeriod ? _RSISlowCache[i] - _RSISlowCache[i - CompositeRSIMomentumPeriod] : 0;
        double RSISMA = GetSMA(_RSIFastCache, i - CompositeRSISMAPeriod, 0, CompositeRSISMAPeriod);
        _CompositeCache.push_back(RSIDelta + RSISMA);
    }

    _FastMACache.reserve(Bars());
    _SlowMACache.reserve(Bars());
    for (size_t i = 0; i < Bars(); i++) {
        _FastMACache.push_back(GetSMA(_CompositeCache, i - CompositeFastSMAPeriod, 0, CompositeFastSMAPeriod));
        _SlowMACache.push_back(GetSMA(_CompositeCache, i - CompositeSlowSMAPeriod, 0, CompositeSlowSMAPeriod));
    }
}

//---------------------------------------------------------------------------
// Calculate requested bar
//---------------------------------------------------------------------------
EXPORT void __stdcall Calculate(int index)
{
    std::stringstream key; key << Bars() << "," << Timeframe() << "," << Symbol();
    PrecalculateIfChanged(key.str());

    // like MT4, ForexTester is indexing from most recent to oldest
    int i = Bars() - 1 - index;

    Composite[index] = _CompositeCache[i];
    FastMA[index] = _FastMACache[i];
    SlowMA[index] = _SlowMACache[i];
}
