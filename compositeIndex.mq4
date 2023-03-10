//+------------------------------------------------------------------
#property copyright   "ambientlight"
#property link        "noreply@noreply.com"
#property description "Composite index"
//+------------------------------------------------------------------
#property indicator_separate_window
#property indicator_buffers 5
#property indicator_plots   3
#property indicator_label1  "Composite index"
#property indicator_type1   DRAW_LINE
#property indicator_color1  clrDarkGray
#property indicator_width1  2
#property indicator_label2  "Composite index Fast SMA"
#property indicator_type2   DRAW_LINE
#property indicator_color2  clrLimeGreen
#property indicator_width2  2
#property indicator_label3  "Composite index Slow SMA"
#property indicator_type3   DRAW_LINE
#property indicator_color3  clrOrangeRed
#property indicator_width3  2
//--- input parameters
input ENUM_APPLIED_PRICE inpPrice           = PRICE_CLOSE; // Price 
input int                inpRsiSlowPeriod   =  14;         // RSI slow period
input int                inpRsiFastPeriod   =   3;         // RSI fast period
input int                inpMomentumPeriod  =   9;         // Momentum period
input int                inpMaPeriod1       =   3;         // Average period 1
input int                inpMaPeriod2       =  13;         // Average period 2
input int                inpMaPeriod3       =  33;         // Average period 3
//--- buffers declarations
double val1[],val2[],val3[],rsislow[],rsifast[];
//--- indicator handles

//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit(){
  //--- indicator buffers mapping
  SetIndexBuffer(0,val1,INDICATOR_DATA);
  SetIndexBuffer(1,val2,INDICATOR_DATA);
  SetIndexBuffer(2,val3,INDICATOR_DATA);
  SetIndexBuffer(3,rsislow,INDICATOR_CALCULATIONS);
  SetIndexBuffer(4,rsifast,INDICATOR_CALCULATIONS);
  //--- make sure array indexing is no-reversed
  ArraySetAsSeries(val1,false);
  ArraySetAsSeries(val2,false);
  ArraySetAsSeries(val3,false);
  ArraySetAsSeries(rsislow,false);
  ArraySetAsSeries(rsifast,false);
  
  //--- indicator short name assignment
  IndicatorSetString(INDICATOR_SHORTNAME,"Composite index ("+(string)inpRsiSlowPeriod+","+(string)inpRsiFastPeriod+")");
  return (INIT_SUCCEEDED);
}

//+------------------------------------------------------------------+
//| Custom indicator de-initialization function                      |
//+------------------------------------------------------------------+
void OnDeinit(const int reason){
}

//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,const int prev_calculated,const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[]){
  
  if(Bars(_Symbol,_Period) < rates_total){
    return(prev_calculated);
  }
  
  int i = (int)MathMax(prev_calculated-1,0); 
  for(; i < rates_total && !_StopFlag; i++){
    rsislow[i] = iRSI(_Symbol, 0, inpRsiSlowPeriod, inpPrice, rates_total - i - 1);
    rsifast[i] = iRSI(_Symbol, 0, inpRsiFastPeriod, inpPrice, rates_total - i - 1);
    
    double RSIDelta  = (i>=inpMomentumPeriod) ? rsislow[i]-rsislow[i-inpMomentumPeriod] : 0;
    double RSIsma    = iSma(rsifast[i],inpMaPeriod1,i,rates_total,0);
    double Composite = RSIDelta+RSIsma;
    val1[i] = Composite;
    val2[i] = iSma(Composite,inpMaPeriod2,i,rates_total,1);
    val3[i] = iSma(Composite,inpMaPeriod3,i,rates_total,1);
  }

  return (i);
}

//+------------------------------------------------------------------+
//| Custom functions                                                 |
//+------------------------------------------------------------------+
double workSma[][3];
//
//---
//
double iSma(double price,int period,int r,int _bars,int instanceNo=0){
   
  if(ArrayRange(workSma,0)!=_bars)
    ArrayResize(workSma,_bars);
  
  workSma[r][instanceNo] = price;
  double avg = price; 
  int k = 1;
  for(; k < period && (r-k) >= 0; k++) 
    avg += workSma[r-k][instanceNo];

  return(avg/(double)k);
}
//+------------------------------------------------------------------+
