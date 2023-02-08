//----------------------------------------------------------------------------
// Indicator interface unit ver 1.13, (c) Forex Tester Software 2012
//
// What's new:
//
// ver 1.13
//
//   1. Added new function ObjectGetText - read the object's description
//
// ver 1.12
//
//   1. Added new function Bid - returns current bid price
//   2. Added new function Ask - returns current ask price
//   3. Added new function TimeCurrent - returns current time
//
// ver 1.11
//
//   1. Added new function CreateLoopbackIndexBuffer - creates special loopback
//      index buffer with limited size
//   2. Added new procedure AddBufferValue - adds value to a loopback buffer
//
//
// ver 1.10a
//
//   1. Added new function ChartToScrX - converts index to screen x coordinate
//   2. Added new function ChartToScrY - converts price to screen y coordinate
//   3. Added new function ScrToChartX - converts screen x coordinate to index
//   4. Added new function ScrToChartY - converts screen y coordinate to price
//   5. Added new function GetChartInfo - returns information about chart
//   6. New type TChartInfo
//
// ver 1.10 (Mar 2011 update by Wessel de Roode)
//   Main focus is get the API more C++ usable like overloading functions etc.
//
//   1. Added 128 official cL standard color names
//   2. Added cl_Transparent for coloring transparent
//   3. Added new time frames: PERIOD_M30,M39,H2 to H23,MN,MN1
//   4. Added overloading: Print(string), Print(int), Print(double), Print(stringstream& s)
//   5. Added overloading: SetIndexLabel(int index, string s), SetIndexLabel(int index, stringstream& s)
//   6. Added overloading: ObjectSetText stringstream& s, string, double and int.
//   7. Added overloading: AddOptionValue stringstream& s, string

//   Bugfixes
//   1. Changed ot_double into ot_Double to make it consistent with the
//      other TOptionType enum types since C++ is case sensitive it matters.
//      Left the old version ot_double as well for backward compatibility
//   2. Replaced obsolete/insecure strcpy into strcpy_s (no more compiler warnings)
//   3. Wrong color definition of clGreen replaced RGB(0,0xFF,0) into 0x008000
//      Using green would show up as LimeGreen in ForexTester now shows up as Green
//   4. clMaroon missing in C++ and available in Delphi, fixed
//   5. Incorrect type fix changed cl_GridColor  from $01000000 into 0x01000000L
//
// ver 1.9
// 
//   1. Added new funtion ObjectsTotal - returns number of objects on 
//      associated chart
//   2. Added new function ObjectName - returns object's name by
//      its index
//   3. Added new function iBarShift - returns index of a bar by its time
//   4. Added new function iHighest - returns index of a bar with the
//      highest value
//   5. Added new function iLowest - returns index of a bar with the
//      lowest value
//   6. New constants MODE_OPEN .. MODE_TIME
//   7. Added new external parameter type - ot_Color, represents color
//
// ver 1.7
//   1. Constants of timeframes changed
//                   now      before
//      PERIOD_M1  = 1;       0
//      PERIOD_M5  = 5;       1
//      PERIOD_M15 = 15;      2
//      PERIOD_H1  = 60;      3
//      PERIOD_H4  = 60*4;    4
//      PERIOD_D1  = 60*24;   5
//      PERIOD_W1  = 60*24*7; 6
//
//   2. Timeframe should be defined with number of minutes in it
//
// ver 1.6
//
//   1. Added new styles: ds_HystogramFill and ds_ColoredCandles
//
// ver 1.5
//   1. Found error in SetBufferValue - missed stdcall declaration
//   2. Added new procedure SetBufferShift
//   3. Added new procedure OnParamsChange that is called after
//      every change in indicator parameters
//   4. Now procedures Done, OnParamsChange could be omitted if
//      unnecessary 
//
// ver 1.4
//   1. Added new object functions
//      ObjectCreate
//      ObjectDelete
//      ObjectExists
//      ObjectType
//      ObjectSet
//      ObjectGet
//      ObjectsDeleteAll
//      ObjectSetText
//   2. Added function to get interface verion - GetInterfaceVersion
//
// ver 1.3
//   1. Added new function SetIndexLabel
//   2. New draw style (TDrawStyle) ds_Section
//
// ver 1.2
//   1. Added new constant cl_GridColor - to paint indicator levels
//      with current grid color
//   2. Added new functions
//      IndicatorDigits - indicator precision
//      Timeframe - indicator timeframe
//
// ver 1.1
//   1. New interface function SetIndexSymbol
//   2. New draw style (TDrawStyle) ds_Symbol
//
// ver 1.0
//
//----------------------------------------------------------------------------

#include <vector>
#include <string>
#include <sstream>

#define EXPORT __declspec(dllexport)

#define PChar char*
#define TDateTime double
#define TColor unsigned long
#define PCurrencyInfo TCurrencyInfo*
#define PInterfaceProcRec TInterfaceProcRec*
#define PLineStyleRec TLineStyleRec*

typedef unsigned short    Word;
typedef unsigned long     LongWord;

enum TPenStyle {psSolid, psDash, psDot, psDashDot, psDashDotDot, psClear, psInsideFrame};

#define clRed					RGB(0xFF,0,0) 
#define clGreen					0x008000 
#define clBlue					RGB(0,0,0xFF) 
#define clYellow				RGB(0xFF,0xFF,0)
#define clLime					RGB(0x00,0xFF,0x00)
#define clWhite					RGB(0xFF,0xFF,0xFF)

// Extended colors
#define clAliceBlue             0xFFF8F0
#define clAntiqueWhite          0xD7EBFA
#define clAquamarine            0xD4FF7F
#define clAzure                 0xFFFFF0
#define clBeige                 0xDCF5F5
#define clBrown					0x2A2AA5
#define clBisque                0xC4E4FF
#define clBlanchedAlmond        0xCDEBFF
#define clBlueViolet            0xE22B8A
#define clBrown                 0x2A2AA5
#define clBurlyWood             0x87B8DE
#define clCadetBlue             0xA09E5F
#define clChartreuse            0x00FF7F
#define clChocolate             0x1E69D2
#define clCoral                 0x507FFF
#define clCornflowerBlue        0xED9564
#define clCornsilk              0xDCF8FF
#define clCrimson               0x3C14DC
#define clCyan                  0xFFFF00
#define clDarkBlue              0x8B0000
#define clDarkCyan              0x8B8B00
#define clDarkGoldenrod         0x0B86B8
#define clDarkGray              0xA9A9A9
#define clDarkGreen             0x006400
#define clDarkKhaki             0x6BB7BD
#define clDarkMagenta           0x8B008B
#define clDarkOliveGreen        0x2F6B55
#define clDarkOrange            0x008CFF
#define clDarkOrchid            0xCC3299
#define clDarkRed               0x00008B
#define clDarkSalmon            0x7A96E9
#define clDarkSeaGreen          0x8BBC8F
#define clDarkSlateBlue         0x8B3D48
#define clDarkSlateGray         0x4F4F2F
#define clDarkTurquoise         0xD1CE00
#define clDarkViolet            0xD30094
#define clDeepPink              0x9314FF
#define clDeepSkyBlue           0xFFBF00
#define clDimGray               0x696969
#define clDodgerBlue            0xFF901E
#define clFirebrick             0x2222B2
#define clFloralWhite           0xF0FAFF
#define clForestGreen           0x228B22
#define clGainsboro             0xDCDCDC
#define clGhostWhite            0xFFF8F8
#define clGold                  0x00D7FF
#define clGoldenrod             0x20A5DA
#define clGreenYellow           0x2FFFAD
#define clHoneydew              0xF0FFF0
#define clHotPink               0xB469FF
#define clIndianRed             0x5C5CCD
#define clIndigo                0x82004B
#define clIvory                 0xF0FFFF
#define clKhaki                 0x8CE6F0
#define clLavender              0xFAE6E6
#define clLavenderBlush         0xF5F0FF
#define clLawnGreen             0x00FC7C
#define clLemonChiffon          0xCDFAFF
#define clLightBlue             0xE6D8AD
#define clLightCoral            0x8080F0
#define clLightCyan             0xFFFFE0
#define clLightGoldenrodYellow  0xD2FAFA
#define clLightGray             0xD3D3D3
#define clLightGreen            0x90EE90
#define clLightPink             0xC1B6FF
#define clLightSalmon           0x7AA0FF
#define clLightSeaGreen         0xAAB220
#define clLightSkyBlue          0xFACE87
#define clLightSlateGray        0x998877
#define clLightSteelBlue        0xDEC4B0
#define clLightYellow           0xE0FFFF
#define clLimeGreen             0x32CD32
#define clLinen                 0xE6F0FA
#define clMagenta               0xFF00FF
#define clMaroon				        0x000080	
#define clMediumAquamarine      0xAACD66
#define clMediumBlue            0xCD0000
#define clMediumOrchid          0xD355BA
#define clMediumPurple          0xDB7093
#define clMediumSeaGreen        0x71B33C
#define clMediumSlateBlue       0xEE687B
#define clMediumSpringGreen     0x9AFA00
#define clMediumTurquoise       0xCCD148
#define clMediumVioletRed       0x8515C7
#define clMidnightBlue          0x701919
#define clMintCream             0xFAFFF5
#define clMistyRose             0xE1E4FF
#define clMoccasin              0xB5E4FF
#define clNavajoWhite           0xADDEFF
#define clOldLace               0xE6F5FD
#define clOliveDrab             0x238E6B
#define clOrange                0x00A5FF
#define clOrangeRed             0x0045FF
#define clOrchid                0xD670DA
#define clPaleGoldenrod         0xAAE8EE
#define clPaleGreen             0x98FB98
#define clPaleTurquoise         0xEEEEAF
#define clPaleVioletRed         0x9370DB
#define clPapayaWhip            0xD5EFFF
#define clPeachPuff             0xB9DAFF
#define clPeru                  0x3F85CD
#define clPink                  0xCBC0FF
#define clPlum                  0xDDA0DD
#define clPowderBlue            0xE6E0B0
#define clPurple                0x800080
#define clRosyBrown             0x8F8FBC
#define clRoyalBlue             0xE16941
#define clSaddleBrown           0x13458B
#define clSalmon                0x7280FA
#define clSandyBrown            0x60A4F4
#define clSeaGreen              0x578B2E
#define clSeaShell              0xEEF5FF
#define clSienna                0x2D52A0
#define clSilver				0xC0C0C0
#define clSkyBlue               0xEBCE87
#define clSlateBlue             0xCD5A6A
#define clSlateGray             0x908070
#define clSnow                  0xFAFAFF
#define clSpringGreen           0x7FFF00
#define clSteelBlue             0xB48246
#define clTan                   0x8CB4D2
#define clThistle               0xD8BFD8
#define clTomato                0x4763FF
#define clTurquoise             0xD0E040
#define clViolet                0xEE82EE
#define clWheat                 0xB3DEF5
#define clWhiteSmoke            0xF5F5F5
#define clYellowGreen           0x32CD9A

// Special code for transparent
#define cl_Transparent       0x1FFFFFFFL

// current color of the grid (AddLevel procedure)
#define cl_GridColor	     0x01000000L


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// option type
enum TOptionType {
  ot_Longword  = 0,
  ot_Integer   = 1,
  ot_double    = 2, // Obsolete!!!, left here for backwards compat. use ot_Double
  ot_Double    = 2, 
  ot_PChar     = 3,
  ot_Boolean   = 4,
  ot_EnumType  = 5,
  ot_TimeFrame = 6,
  ot_Currency  = 7,
  ot_LineStyle = 8,
  ot_Separator = 9,
  ot_Indicator = 10,
  ot_Color     = 11
};

// indicator window
enum TOutputWindow {
  ow_ChartWindow    = 0,
  ow_SeparateWindow = 1
};

// drawing type
enum TDrawStyle {
  ds_Line           = 0,
  ds_Histogram      = 1,
  ds_Fill           = 2,
  ds_Symbol         = 3,
  ds_Section        = 4,
  ds_HistogramFill  = 5,
  ds_ColoredCandles = 6,
  ds_None           = 10
};

// object types
enum TObjectType {
  obj_AnyObject        = 0,
  obj_VLine            = 1,
  obj_HLine            = 2,
  obj_TrendLine        = 3,
  obj_Ray              = 4,
  obj_PolyLine         = 5,
  obj_FiboRetracement  = 6,
  obj_FiboTimeZones    = 7,
  obj_FiboArc          = 8,
  obj_FiboFan          = 9,
  obj_AndrewsPitchfork = 10,
  obj_Text             = 11,
  obj_TextLabel        = 12
};

#pragma pack(push, 1)
using namespace std;
//-------------------------------------------
// currency information
//-------------------------------------------
struct TCurrencyInfo {
  PChar Symbol;               // currency name
  int Digits;                 // number of digits after '.'
  int spread;                 // spread in pips
  double Point;               // minimal currency point
  int lot;                    // 1 lot cost
  PChar curr;                 // lot currency
  double SwapLong;            // swap points long
  double SwapShort;           // swap points short
};

//-------------------------------------------
// indicator option record
//-------------------------------------------
struct TIndicatorOption {
  PChar ParamsStr;            // params string
  void* BufferPtr;            // pointer to buffer object
};

//-------------------------------------------
// chart info
//-------------------------------------------
struct TChartInfo {
  int dwSize;
  int FirstIndex;
  int LastIndex;
  RECT PaintRect;
  int BarWidth;
};

//-------------------------------------------
// line style
//-------------------------------------------
struct TLineStyleRec{
    TColor color;                 // line color
    TPenStyle style;              // line style
    int width;                    // line width
	TDrawStyle DrawingStyle;      // drawing style on chart
    int Symbol;                   // symbol index for ds_Symbol
    int xoffs, yoffs;             // x and y shift of symbol in pixels
};

#pragma pack(pop)


//-------------------------------------------
// buffer item
//-------------------------------------------
class TIndexBuffer;

class TBufferItem
{
friend class TIndexBuffer;

private:
  TIndexBuffer* ptr;
  int index;

public:
  TBufferItem();

  operator double();
  TBufferItem& operator= (double value);
};

//-------------------------------------------
// index buffer
//-------------------------------------------
class TIndexBuffer
{
private:
  int fBufferIndex;

public:
  TIndexBuffer();
  TIndexBuffer(int index);
  
  int Count();
  double GetMax(int index1, int index2);
  double GetMin(int index1, int index2);
  int& GetBufferIndex();

  double GetValue(int index);
  void SetValue(int index, const double value);

  TBufferItem& operator[] (int index);
};


//-------------------------------------------
// Buffers array
//-------------------------------------------
class TBuffersArray
{
private:
  TIndexBuffer* GetBuffer(int index);

public:
  TIndexBuffer* operator[] (int index);

  //property    data[index: int]: TIndexBuffer read GetBuffer; default;
};


//-------------------------------------------
// TObjectList class
//-------------------------------------------
class TObjectList 
{
private:
  std::vector<TIndexBuffer> data;

public:
  int Count();
  void Add(TIndexBuffer buffer);
  TIndexBuffer* Get(int index);
  
  TIndexBuffer* operator[] (int index);
};


// timeframes
#define PERIOD_M1				1
#define PERIOD_M5				5
#define PERIOD_M15				15
#define PERIOD_M30				30    
#define PERIOD_M39				39    // 10 bars on the US Stock Exchange in one day
#define PERIOD_H1				60*1
#define PERIOD_H2				60*2
#define PERIOD_H3				60*3
#define PERIOD_H4				60*4
#define PERIOD_H5				60*5
#define PERIOD_H6				60*6
#define PERIOD_H7				60*7
#define PERIOD_H8				60*8
#define PERIOD_H9				60*9
#define PERIOD_H10				60*10
#define PERIOD_H11				60*11
#define PERIOD_H12				60*12
#define PERIOD_H13				60*13
#define PERIOD_H14				60*14
#define PERIOD_H15				60*15
#define PERIOD_H16				60*16
#define PERIOD_H17				60*17
#define PERIOD_H18				60*18
#define PERIOD_H19				60*19
#define PERIOD_H20				60*20
#define PERIOD_H21				60*21
#define PERIOD_H22				60*22
#define PERIOD_H23				60*23
#define PERIOD_D1				60*24
#define PERIOD_W1				60*24*7
#define PERIOD_MN				43200
#define PERIOD_MN1				43200

// object properties constants
#define OBJPROP_TIME1            0
#define OBJPROP_PRICE1           1
#define OBJPROP_TIME2            2
#define OBJPROP_PRICE2           3
#define OBJPROP_TIME3            4
#define OBJPROP_PRICE3           5
#define OBJPROP_COLOR            6
#define OBJPROP_STYLE            7
#define OBJPROP_WIDTH            8
#define OBJPROP_ELLIPSE          11
#define OBJPROP_FIBOCLOSEDENDS   12
#define OBJPROP_FIBOSHOWPRICE    13
#define OBJPROP_FIBOENDWIDTH     14
#define OBJPROP_FIBOLEVELS       15
#define OBJPROP_FIBOLEVELN       16
#define OBJPROP_LEVELCOLOR       17
#define OBJPROP_LEVELSTYLE       18
#define OBJPROP_LEVELWIDTH       19
#define OBJPROP_LEVELVALUE       20
#define OBJPROP_FONTSIZE         21
#define OBJPROP_HALIGNMENT       22
#define OBJPROP_VALIGNMENT       23
#define OBJPROP_FONTNAME         24
#define OBJPROP_XDISTANCE        25
#define OBJPROP_YDISTANCE        26
#define OBJPROP_TEXT             27
#define OBJPROP_NAME             28
#define OBJPROP_SCREENCOORDS     29
#define OBJPROP_SCRHALIGNMENT    30
#define OBJPROP_SCRVALIGNMENT    31
#define OBJPROP_FILLINSIDE       32
#define OBJPROP_FILLCOLOR        33

// text alignment
#define tlTop             0
#define tlCenter          1
#define tlBottom          2

#define taLeftJustify     0
#define taRightJustify    1
#define taCenter          2

#define MODE_OPEN    0
#define MODE_LOW     1
#define MODE_HIGH    2
#define MODE_CLOSE   3
#define MODE_VOLUME  4
#define MODE_TIME    5


// Get currency information
// Symbol - currency name
// info - pointer to TCurrencyInfo record
// result: true if success
bool GetCurrencyInfo(PChar Symbol, PCurrencyInfo& info);

// Register option
// OptionName - name of option
// OptionType - see TOptionType enumeration
// option - pointer to first byte
void RegOption(PChar OptionName, TOptionType OptionType, void* option);

// Add separator to options dialog
// text - separator caption
void AddSeparator(PChar text);

// Add option value (for drop down box options)
// OptionName - name of previously registered option
// value - new value in drop down box
void AddOptionValue(PChar OptionName, PChar value);
void AddOptionValue(PChar OptionName, string s);
void AddOptionValue(PChar OptionName, stringstream& s);

// Set option range (for int, longword and double types)
// OptionName - name of previously registered option
// LowValue - lowest value in range
// HighValue - highest value in range
// note: in dialog box after entering option value it will be checked
//       to be in range LowValue <= value <= HighValue
void SetOptionRange(PChar OptionName, double LowValue, double HighValue);

// Set option digits (for double options) defines number of digits after point
// OptionName - name of option
// digits - number of digits after point
void SetOptionDigits(PChar OptionName, Word digits);

// Print text in "Journal" window in ForexTester
void Print(PChar s);        // s - PChar
void Print(string s);		// s - string
void Print(stringstream& s);// s - stringstream 
void Print(int i);          // s - int
void Print(double d);       // s - double

// Set indicator short name
// name - short name
void IndicatorShortName(PChar name);

// Set number of index buffers
// count - number of buffers
void IndicatorBuffers(int count);

// Create index buffer
// result - buffer id
TIndexBuffer CreateIndexBuffer();

// Set index buffer
// index - number of index buffer slot
// buffer - buffer id
void SetIndexBuffer(int index, TIndexBuffer buffer);

// Set index buffer style
// index - number of index buffer slot
// _type - paint type
// style - line style
// width - line width
// color - line color
void SetIndexStyle(int index, TDrawStyle DrawStyle, TPenStyle style, int width, TColor color);

// Set index symbol
// index - number of an index buffer slot
// symbol - number of symbol in Wingdings font
// xoffs - x offset from center point of the symbol in pixels
// yoffs - y offset from center point of the symbol in pixels
void SetIndexSymbol(int index, int symbol, int xoffs = 0, int yoffs = 0);

// Set index label
// index - number of an index buffer slot
// text - text label
void SetIndexLabel(int index, PChar text);
void SetIndexLabel(int index, string s);
void SetIndexLabel(int index, stringstream& s);

// Set indicator output window
// aWindow - where to paint indicator (ow_ChartWindow, ow_SeparateWindow)
void SetOutputWindow(TOutputWindow aWindow);

// Set fixed maximum/minimum for indicator window
// aMin - minimum value
// aMax - maximum value
void SetFixedMinMaxValues(double aMin, double aMax);

// Add level line to indicator window
// value - level value
// style - line style
// width - line width
// color - line color
void AddLevel(double value, TPenStyle style, int width, TColor color);

// Set empty value that will not be painted
// value - new empty value
void SetEmptyValue(double value);

// Currency name
PChar Symbol();

// Currency digits after point
int Digits();

// Minimal currency point
double Point();

// Get open value
// Symbol - requested currency
// TimeFrame - requested timeframe
// index - shift in bars from last bar
double iOpen(PChar Symbol, int TimeFrame, int index);

// Get close value
// Symbol - requested currency
// TimeFrame - requested timeframe
// index - shift in bars from last bar
double iClose(PChar Symbol, int TimeFrame, int index);

// Get high value
// Symbol - requested currency
// TimeFrame - requested timeframe
// index - shift in bars from last bar
double iHigh(PChar Symbol, int TimeFrame, int index);

// Get low value
// Symbol - requested currency
// TimeFrame - requested timeframe
// index - shift in bars from last bar
double iLow(PChar Symbol, int TimeFrame, int index);

// Get volume
// Symbol - requested currency
// TimeFrame - requested timeframe
// index - shift in bars from last bar
double iVolume(PChar Symbol, int TimeFrame, int index);

// Get time of bar
// Symbol - requested currency
// TimeFrame - requested timeframe
// index - shift in bars from last bar
TDateTime iTime(PChar Symbol, int TimeFrame, int index);

// Get number of bars
// Symbol - requested currency
// TimeFrame - requested timeframe
int iBars(PChar Symbol, int TimeFrame);

// Get open value in default currency and timeframe
// index - shift in bars from last bar
double Open(int index);

// Get close value in default currency and timeframe
// index - shift in bars from last bar
double Close(int index);

// Get high value in default currency and timeframe
// index - shift in bars from last bar
double High(int index);

// Get low value in default currency and timeframe
// index - shift in bars from last bar
double Low(int index);

// Get volume in default currency and timeframe
// index - shift in bars from last bar
double Volume(int index);

// Get time of bar in default currency and timeframe
// index - shift in bars from last bar
TDateTime Time(int index);

// Get number of bars in default currency and timeframe
int Bars();

// Set indicator precision (number of digits after .)
// digits - number of digits after point
void IndicatorDigits(int digits);

// Get indicator timeframe
int Timeframe();

// Get interface version
void GetInterfaceVersion(int& MajorValue, int& MinorValue);

// Set buffer shift in bars
void SetBufferShift(int index, int Shift);

// Create object
// name - object name (must be unique)
// ObjType - type of object (see TObjectType)
// window - in what window to show object (ignored)
// time1, price1 .. time3, price3 - time and price coordinates of object
// function returns true if successful
bool ObjectCreate(PChar name, TObjectType ObjType, int window,
  TDateTime time1, double price1, TDateTime time2 = 0, double price2 = 0, 
  TDateTime time3 = 0, double price3 = 0);

// Delete object by name
// name - name of the object
void ObjectDelete(PChar name);

// Check if object already exists
// name  - name of the object
bool ObjectExists(PChar name);

// Get object type
// name - name of the object
TObjectType ObjectType(PChar name);

// Set object property
// name - name of the object
// index - property index
// value - new value
// function returns true if successful
bool ObjectSet(PChar name, int index, double value);

// Get object property
// name - name of the object
// index - property index
double ObjectGet(PChar name, int index);

// Delete all objects
// window - window where to delete
// ObjType - type of objects
void ObjectsDeleteAll(int window = 0, TObjectType ObjType = obj_AnyObject);

// set text/description for object
// name - name of the object
// text - text to set
// FontSize - font size
// FontName - font name
// Color - font color
// function returns true if successful
bool ObjectSetText(PChar name, PChar text, int FontSize = 12,
    const PChar FontName = "Arial", TColor Color = clRed);
bool ObjectSetText(PChar name, string string, int FontSize = 12,
    const PChar FontName = "Arial", TColor Color = clRed);
bool ObjectSetText(PChar name, stringstream& string, int FontSize = 12,
    const PChar FontName = "Arial", TColor Color = clRed);
bool ObjectSetText(PChar name, double d, int FontSize = 12,
    const PChar FontName = "Arial", TColor Color = clRed);
bool ObjectSetText(PChar name, int i, int FontSize = 12,
    const PChar FontName = "Arial", TColor Color = clRed);

// get text/description of the object
// name - name of the object
// function returns text or empty string if failed
PChar ObjectGetText(PChar name);

// get buffer value
double GetIndicatorBufferValue(TIndicatorOption indicator, int index);

// get number of graphical objects on associated chart
int  ObjectsTotal();

// get object's name by its index
// index - object's index
PChar ObjectName(int index);

// get bar index by its time
// Symbol - requested currency
// TimeFrame - requested timeframe
// time - time of the bar
// Exact - if this parameter is true then time should be exactly the same
//         otherwise will be returned index of the bar where time is
//         between time[index] and time[index + 1]
// function returns index of the bar if successful, and -1 if failed
int iBarShift(PChar Symbol, int TimeFrame, TDateTime time, bool Exact);

// get highest value in array
// Symbol - requested currency
// TimeFrame - requested timeframe
// _type - type of value (see constants MODE_OPEN .. MODE_TIME)
// count - number of bars to search
// index - first index to start searching
// function returns index of the bar if successful, and -1 if failed
int iHighest(PChar Symbol, int TimeFrame, int _type, int count, int index);

// get lowest value in array
// Symbol - requested currency
// TimeFrame - requested timeframe
// _type - type of value (see constants MODE_OPEN .. MODE_TIME)
// count - number of bars to search
// index - first index to start searching
// function returns index of the bar if successful, and -1 if failed
int iLowest(PChar Symbol, int TimeFrame, int _type, int count, int index);

// convert chart index to screen x coordinate
// index - index of a bar
// function returns x coordinate if succesful, and 0 if failed
int ChartToScrX(int index);

// convert chart price to screen y coordinate
// price - price value
// function returns y coordinate if succesful, and 0 if failed
int ChartToScrY(double price);

// convert screen x coordinate to chart bar's index
// x - screen x coordinate
// function returns index if succesful, and -1 if failed
int ScrToChartX(int x);

// convert screen y coordinate to chart price
// y - screen y coordinate
// function returns price value if succesful, and -1 if failed
double ScrToChartY(int y);

// get information about chart
// function returns true and filled ChartInfo structure if succesful, and
// false if faled
bool GetChartInfo(TChartInfo& ChartInfo);

// get current Bid price
double Bid();

// get current Ask price
double Ask();

// get current time
TDateTime TimeCurrent();

void SetCalcModeMT4();
int mql4_IndicatorCounted();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//implementation
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// indicator interface procedures
typedef bool (__stdcall *TGetCurrencyInfoFunc) (void* ptr, PChar Symbol, PCurrencyInfo& info);
typedef void (__stdcall *TSetPropertyProc) (void* ptr, int PropertyID, void* value);
typedef void (__stdcall *TRegOptionProc) (void* ptr, PChar OptionName, int OptionType, void* OptionPtr);
typedef void (__stdcall *TAddOptionValueProc) (void* ptr, PChar OptionName, PChar value);
typedef void (__stdcall *TSetOptionRange) (void* ptr, PChar OptionName, double LowValue, double HighValue);
typedef void (__stdcall *TSetOptionDigitsProc) (void* ptr, PChar OptionName, Word digits);
typedef void (__stdcall *TPrintProc) (void* ptr, PChar s);
typedef double (__stdcall *TGetDoubleFunc) (void* ptr);
typedef PChar (__stdcall *TGetPCharFunc) (void* ptr);
typedef int (__stdcall *TGetIntegerFunc) (void* ptr);
typedef double (__stdcall *TGet_iOHLCVFunc) (void* ptr, PChar Symbol, LongWord TimeFrame, int index);
typedef TDateTime (__stdcall *TGet_iTimeFunc) (void* ptr, PChar Symbol, LongWord TimeFrame, int index);
typedef LongWord (__stdcall *TGet_iBarsFunc) (void* ptr, PChar Symbol, LongWord TimeFrame);
typedef double (__stdcall *TGetOHLCVFunc) (void* ptr, int index);
typedef TDateTime (__stdcall *TGetTimeFunc) (void* ptr, int index);
typedef LongWord (__stdcall *TGetBarsFunc) (void* ptr);
typedef void (__stdcall *TIndicatorShortNameProc) (void* ptr, PChar name);
typedef void (__stdcall *TIndicatorBuffersProc) (void* ptr, int count);
typedef int (__stdcall *TCreateIndexBufferFunc) (void* ptr);
typedef void (__stdcall *TSetIndexBufferProc) (void* ptr, int index, int buffer);
typedef void (__stdcall *TSetIndexStyleProc) (void* ptr, int index, int _type, int _style, int width, int clr);
typedef double (__stdcall *TGetBufferValueFunc) (void* ptr, int buffer, LongWord index);
typedef void (*TSetBufferValueProc) (void* ptr, int buffer, LongWord index, double value);
typedef void (__stdcall *_TSetBufferValueProc) (void* ptr, int buffer, LongWord index, double value);
typedef LongWord (__stdcall *TGetBufferCountFunc) (void* ptr, int buffer);
typedef double (__stdcall *TGetBufferMaxFunc) (void* ptr, int buffer, LongWord index1, LongWord index2);
typedef double (__stdcall *TGetBufferMinFunc) (void* ptr, int buffer, LongWord index1, LongWord index2);
typedef void (__stdcall *TSetOutputWindowProc) (void* ptr, int aWindow);
typedef void (__stdcall *TSetFixedMinMaxValuesProc) (void* ptr, double aMin, double aMax);
typedef void (__stdcall *TAddLevelProc) (void* ptr, double value, int _style, int width, int clr);
typedef void (__stdcall *TSetEmptyValueProc) (void* ptr, double value);
typedef void (__stdcall *TSetIndexSymbolProc) (void* ptr, int index, int symbol, int xoffs, int yoffs);
typedef void (__stdcall *TIndicatorDigitsProc) (void* ptr, int digits);
typedef void (__stdcall *TSetIndexLabelProc) (void* ptr, int index, PChar text);
typedef void (__stdcall *TGetInterfaceVersionProc) (void* ptr, int& MajorValue, int& MinorValue);
typedef bool (__stdcall *TObjectCreateFunc) (void* ptr, PChar name, int ObjType, int window,
  TDateTime time1, double price1, TDateTime time2, double price2, TDateTime time3, double price3);
typedef void (__stdcall *TObjectDeleteProc) (void* ptr, PChar name);
typedef bool (__stdcall *TObjectExistsFunc) (void* ptr, PChar name);
typedef int (__stdcall *TObjectTypeFunc) (void* ptr, PChar name);
typedef bool (__stdcall *TObjectSetFunc) (void* ptr, PChar name, int index, double value);
typedef double (__stdcall *TObjectGetFunc) (void* ptr, PChar name, int index);
typedef void (__stdcall *TObjectsDeleteAllProc) (void* ptr, int window, int ObjType);
typedef bool (__stdcall *TObjectSetTextFunc) (void* ptr, PChar name, PChar text, int FontSize,
  PChar FontName, int Color);
typedef void (__stdcall *TSetBufferShift) (void* ptr, int index, int Shift);
typedef double (__stdcall *TGetIndicatorBufferValueFunc) (void* ptr, TIndicatorOption indicator, int index);
typedef PChar (__stdcall *TObjectNameFunc) (void* ptr, int index);
typedef int (__stdcall *TIBarShiftFunc) (void* ptr, PChar Symbol, int TimeFrame, TDateTime time, bool Exact);  
typedef int (__stdcall *TIHighestFunc) (void* ptr, PChar Symbol, int TimeFrame, int _type, int count, int index);
typedef int (__stdcall *TILowestFunc) (void* ptr, PChar Symbol, int TimeFrame, int _type, int count, int index);
typedef int (__stdcall *TChartToScrXFunc) (void* ptr, int index);
typedef int (__stdcall *TChartToScrYFunc) (void* ptr, double price);
typedef int (__stdcall *TScrToChartXFunc) (void *ptr, int x);
typedef double (__stdcall *TScrToChartYFunc) (void* ptr, int y);
typedef bool (__stdcall *TGetChartInfoFunc) (void* ptr, TChartInfo& ChartInfo);
typedef int (__stdcall *TCreateLoopbackIndexBufferFunc) (void* ptr, int MaxSize);
typedef void (__stdcall *TAddBufferValueProc) (void* ptr, int buffer, TDateTime DateTime, double value);
typedef int (__stdcall *TGetBufferItemsCountFunc) (void* ptr, int buffer);
typedef PChar (__stdcall *TObjectGetTextFunc) (void* ptr, PChar name);
typedef void (__stdcall *TSetCalcModeMT4Proc) (void* ptr);
typedef int (__stdcall *Tmql4_IndicatorCountedFunc) (void* ptr);

#pragma pack(push)

// pointers to interface procedures
struct TInterfaceProcRec {
  LongWord dwSize;

  TGetCurrencyInfoFunc GetCurrencyInfo;
  void* pGetCurrencyInfo;

  TRegOptionProc RegOption;
  void* pRegOption;

  TAddOptionValueProc AddOptionValue;
  void* pAddOptionValue;
  
  TSetOptionRange SetOptionRange;
  void* pSetOptionRange;

  TSetOptionDigitsProc SetOptionDigits;
  void* pSetOptionDigits;

  TPrintProc Print;
  void* pPrint;

  TIndicatorShortNameProc IndicatorShortName;
  void* pIndicatorShortName;

  TIndicatorBuffersProc IndicatorBuffers;
  void* pIndicatorBuffers;

  TCreateIndexBufferFunc CreateIndexBuffer;
  void* pCreateIndexBuffer;

  TSetIndexBufferProc SetIndexBuffer;
  void* pSetIndexBuffer;

  TSetIndexStyleProc SetIndexStyle;
  void* pSetIndexStyle;

  TSetOutputWindowProc SetOutputWindow;
  void* pSetOutputWindow;

  TSetFixedMinMaxValuesProc SetFixedMinMaxValues;
  void* pSetFixedMinMaxValues;

  TAddLevelProc AddLevel;
  void* pAddLevel;

  TSetEmptyValueProc SetEmptyValue;
  void* pSetEmptyValue;

  TGetBufferValueFunc GetBufferValue;
  void* pGetBufferValue;
  
  TSetBufferValueProc SetBufferValue;
  void* pSetBufferValue;

  TGetBufferCountFunc GetBufferCount;
  void* pGetBufferCount;

  TGetBufferMaxFunc GetBufferMax;
  void* pGetBufferMax;

  TGetBufferMinFunc GetBufferMin;
  void* pGetBufferMin;

  TGetPCharFunc Symbol;
  void* pSymbol;

  TGetIntegerFunc Digits;
  void* pDigits;

  TGetDoubleFunc Point;
  void* pPoint;

  TGet_iOHLCVFunc iOpen;
  void* p_iOpen;

  TGet_iOHLCVFunc iClose;
  void* p_iClose;

  TGet_iOHLCVFunc iHigh;
  void* p_iHigh;

  TGet_iOHLCVFunc iLow;
  void* p_iLow;

  TGet_iOHLCVFunc iVolume;
  void* p_iVolume;
  
  TGet_iTimeFunc iTime;
  void* p_iTime;

  TGet_iBarsFunc iBars;
  void* p_iBars;

  TGetOHLCVFunc Open;
  void* pOpen;

  TGetOHLCVFunc Close;
  void* pClose;

  TGetOHLCVFunc High;
  void* pHigh;

  TGetOHLCVFunc Low;
  void* pLow;

  TGetOHLCVFunc Volume;
  void* pVolume;

  TGetTimeFunc Time;
  void* pTime;

  TGetBarsFunc Bars;
  void* pBars;

  // extentions (dwSize > 300)
  TSetIndexSymbolProc SetIndexSymbol;
  void* pSetIndexSymbol;

  // extensions (dwSize > 308)
  TIndicatorDigitsProc IndicatorDigits;
  void* pIndicatorDigits;

  TGetIntegerFunc Timeframe;
  void* pTimeframe;

  // extensions (dwSize > 324)
  TSetIndexLabelProc SetIndexLabel;
  void* pSetIndexLabel;

  // extensions ver 1.4 (dwSize > 332)
  TGetInterfaceVersionProc GetInterfaceVersion;
  void* pGetInterfaceVersion;

  TObjectCreateFunc ObjectCreate;
  void* pObjectCreate;

  TObjectDeleteProc ObjectDelete;
  void* pObjectDelete;

  TObjectExistsFunc ObjectExists;
  void* pObjectExists;

  TObjectTypeFunc ObjectType;
  void* pObjectType;

  TObjectSetFunc ObjectSet;
  void* pObjectSet;

  TObjectGetFunc ObjectGet;
  void* pObjectGet;

  TObjectsDeleteAllProc ObjectsDeleteAll;
  void* pObjectsDeleteAll;

  TObjectSetTextFunc ObjectSetText;
  void* pObjectSetText;

  // extensions ver 1.5 (dwSize > 404)
  _TSetBufferValueProc _SetBufferValue;
  void* p_SetBufferValue;

  TSetBufferShift SetBufferShift;
  void* pSetBufferShift;

  // extensions ver 1.6 (dwSize > 420)
  TGetIndicatorBufferValueFunc GetIndicatorBufferValue;
  void* pGetIndicatorBufferValue;

  // extensions ver 1.9 (dwSize > 428)
  TGetIntegerFunc ObjectsTotal;
  void* pObjectsTotal;

  TObjectNameFunc ObjectName;
  void* pObjectName;

  TIBarShiftFunc iBarShift;
  void* piBarShift;

  TIHighestFunc iHighest;
  void* piHighest;

  TILowestFunc iLowest;
  void* piLowest;

  
  // extensions ver 1.10 (dwSize > 468)
  TChartToScrXFunc ChartToScrX;
  void* pChartToScrX;

  TChartToScrYFunc ChartToScrY;
  void* pChartToScrY;
  
  TScrToChartXFunc ScrToChartX;
  void* pScrToChartX;

  TScrToChartYFunc ScrToChartY;
  void* pScrToChartY;

  TGetChartInfoFunc GetChartInfo;
  void* pGetChartInfo;


  // extensions ver 1.11 (dwSize > 508)
  TCreateLoopbackIndexBufferFunc CreateLoopbackIndexBuffer;
  void* pCreateLoopbackIndexBuffer;

  TAddBufferValueProc AddBufferValue;
  void* pAddBufferValue;

  TGetBufferItemsCountFunc GetBufferItemsCount;
  void* pGetBufferItemsCount;


  // extensions ver 1.12 (dwSize > 532)
  TGetDoubleFunc Bid;
  void* pBid;

  TGetDoubleFunc Ask;
  void* pAsk;

  TGetDoubleFunc TimeCurrent;
  void* pTimeCurrent;

  // extensions ver 1.13 (dwSize > 556)
  TObjectGetTextFunc ObjectGetText;
  void* pObjectGetText;
  
  // extenstions ver 1.14 (dwSize > 564)
  TSetCalcModeMT4Proc SetCalcModeMT4;
  void* pSetCalcModeMT4;
  
  TGetIntegerFunc mql4_IndicatorCounted;
  void* pmql4_IndicatorCounted;
  
};

#pragma pack(pop)


EXPORT TInterfaceProcRec IntrfProcsRec;

TBufferItem buff_item;
TBuffersArray buff;
TObjectList buffers;


BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved) 
{
  switch (dwReason) 
  {
    case DLL_PROCESS_ATTACH:
    {
      memset(&IntrfProcsRec, 0, sizeof(TInterfaceProcRec));
      IntrfProcsRec.dwSize = sizeof(TInterfaceProcRec);
      break;
    }

    case DLL_THREAD_ATTACH:
    break;

    case DLL_THREAD_DETACH:
    break;

    case DLL_PROCESS_DETACH:
    break;
  }

  return TRUE;
}


//-----Replace PChar string with new one--------------------------------------
EXPORT void __stdcall ReplaceStr(PChar& dest, PChar source)
{
  free((void *)dest);
  dest = (PChar)malloc(strlen((char*)source) + 1);
  strcpy_s(dest, strlen((char*)source) + 1, source); // Wessel Improvement
}


///////////////////
// TIndexBuffer //
///////////////////

TIndexBuffer::TIndexBuffer()
{
  fBufferIndex = -1;
}
  
TIndexBuffer::TIndexBuffer(int index)
{
  fBufferIndex = index;
}

int GetBufferCount(int ind)
{
  return IntrfProcsRec.GetBufferCount(IntrfProcsRec.pGetBufferCount, ind); 
}

//-----Get count---------------------------------------------------------------
int TIndexBuffer::Count() 
{ 
  return IntrfProcsRec.GetBufferCount(IntrfProcsRec.pGetBufferCount, fBufferIndex); 
}

//-----Get max value in range--------------------------------------------------
double TIndexBuffer::GetMax(int index1, int index2) 
{
  int cnt = Count() - 1;
  index1 = cnt - index1;
  index2 = cnt - index2;
  if (index1 < 0) index1 = 0;
  if (index2 < 0) index2 = 0;
  
  return IntrfProcsRec.GetBufferMax(IntrfProcsRec.pGetBufferMax, fBufferIndex, index1, index2);
}

//-----Get min value in range--------------------------------------------------
double TIndexBuffer::GetMin(int index1, int index2) 
{
  int cnt = Count() - 1;
  index1 = cnt - index1;
  index2 = cnt - index2;
  if (index1 < 0) index1 = 0;
  if (index2 < 0) index2 = 0;
  
  return IntrfProcsRec.GetBufferMin(IntrfProcsRec.pGetBufferMin, fBufferIndex, index1, index2);
}

//-----Get fBufferIndex value--------------------------------------------------
int& TIndexBuffer::GetBufferIndex()
{
  return fBufferIndex;
}

//-----Get value by index------------------------------------------------------
double TIndexBuffer::GetValue(int index)
{
  index = Count() - 1 - index;
  if (index < 0) index = 0;

  return IntrfProcsRec.GetBufferValue(IntrfProcsRec.pGetBufferValue, fBufferIndex, index);
}

//-----Set value by index------------------------------------------------------
void TIndexBuffer::SetValue(int index, const double value)
{
  index = Count() - 1 - index;
  if (index < 0) index = 0;

  IntrfProcsRec._SetBufferValue(IntrfProcsRec.p_SetBufferValue, fBufferIndex, index, value);
}

TBufferItem& TIndexBuffer::operator[] (int index)
{
  buff_item.ptr = this;
  buff_item.index = index;
  return buff_item;
}


///////////////////
// TBufferItem   //
///////////////////

TBufferItem::TBufferItem()
{ 
  ptr = NULL;
  index = -1;
}

TBufferItem::operator double() 
{ 
  return ptr->GetValue(index);
}

TBufferItem& TBufferItem::operator= (double value) 
{ 
  ptr->SetValue(index, value);
  return *this;
}


///////////////////
// TBuffersArray //
///////////////////

//-----Get buffer--------------------------------------------------------------
TIndexBuffer* TBuffersArray::GetBuffer(int index)
{
  for (int i=0; i < buffers.Count(); i++)
  {
    if (buffers[i]->GetBufferIndex() == index) return buffers[i];
  }

  return NULL;
}

TIndexBuffer* TBuffersArray::operator[] (int index)
{
  return GetBuffer(index);
}


/////////////////
// TObjectList //
/////////////////

int TObjectList::Count()
{
  return data.size();
}

void TObjectList::Add(TIndexBuffer buffer)
{
  data.push_back(buffer);
}

TIndexBuffer* TObjectList::Get(int index)
{
  if (index < 0 || LongWord(index) >= data.size()) return NULL;
  return &data[index];
}

TIndexBuffer* TObjectList::operator[] (int index)
{
  return Get(index);
}


//-----Get currency information------------------------------------------------
bool GetCurrencyInfo(PChar Symbol, PCurrencyInfo& info)
{
  return IntrfProcsRec.GetCurrencyInfo(IntrfProcsRec.pGetCurrencyInfo, Symbol, info);
}

//-----Register option---------------------------------------------------------
void RegOption(PChar OptionName, TOptionType OptionType, void* option)
{
  IntrfProcsRec.RegOption(IntrfProcsRec.pRegOption, OptionName, OptionType, option);
}

//-----Add separator-----------------------------------------------------------
void AddSeparator(PChar text)
{
  IntrfProcsRec.RegOption(IntrfProcsRec.pRegOption, text, ot_Separator, NULL);
}

//-----Add option value (for drop down box options)----------------------------
void AddOptionValue(PChar OptionName, PChar value)
{
  IntrfProcsRec.AddOptionValue(IntrfProcsRec.pAddOptionValue, OptionName, value);
}

void AddOptionValue(PChar OptionName, string s)
{
	char* value;
	value = new char[ s.length()+1 ];
	strcpy_s(value, s.length()+1, s.c_str());
	AddOptionValue( OptionName, value);
	delete[] value;
}

void AddOptionValue(PChar OptionName, stringstream& s)
{
	AddOptionValue(OptionName, s.str() );
}
//-----Set option range (for int, longword and double types)---------------
void SetOptionRange(PChar OptionName, double LowValue, double HighValue)
{
  IntrfProcsRec.SetOptionRange(IntrfProcsRec.pSetOptionRange, OptionName, LowValue, HighValue);
}

//-----Set option digits-------------------------------------------------------
void SetOptionDigits(PChar OptionName, Word digits)
{
  IntrfProcsRec.SetOptionDigits(IntrfProcsRec.pSetOptionDigits, OptionName, digits);
}

//-----Print text in "Journal" window in ForexTester---------------------------
void Print(PChar s)
{
  IntrfProcsRec.Print(IntrfProcsRec.pPrint, s);
}

void Print(string s){
	char* dstP;
	dstP = new char[ s.length()+1 ];
	strcpy_s(dstP, s.length()+1, s.c_str());
	Print( dstP );
	delete[] dstP;
}

void Print(stringstream& s){
	Print( s.str() );
}

void Print(int i){
	std::stringstream s;
	s << i;
	Print(s.str());
}

void Print(double d){
	std::stringstream s;
	s << d; 
	Print(s.str());
}

//-----Set indicator short name------------------------------------------------
void IndicatorShortName(PChar name)
{
  IntrfProcsRec.IndicatorShortName(IntrfProcsRec.pIndicatorShortName, name);
}

//-----Set number of index buffers---------------------------------------------
void IndicatorBuffers(int count)
{
  IntrfProcsRec.IndicatorBuffers(IntrfProcsRec.pIndicatorBuffers, count);
}

//-----Create index buffer-----------------------------------------------------
TIndexBuffer CreateIndexBuffer()
{
  TIndexBuffer buffer = IntrfProcsRec.CreateIndexBuffer(IntrfProcsRec.pCreateIndexBuffer);
  buffers.Add(buffer);

  return buffer;
}

//-----Set index buffer--------------------------------------------------------
void SetIndexBuffer(int index, TIndexBuffer buffer)
{
  IntrfProcsRec.SetIndexBuffer(IntrfProcsRec.pSetIndexBuffer, index, buffer.GetBufferIndex());
}

//-----Set index buffer style--------------------------------------------------
void SetIndexStyle(int index, TDrawStyle DrawStyle, TPenStyle style, int width, TColor color)
{
  IntrfProcsRec.SetIndexStyle(IntrfProcsRec.pSetIndexStyle, index, DrawStyle, style, width, color);
}

//-----Set index buffer symbol-------------------------------------------------
void SetIndexSymbol(int index, int symbol, int xoffs, int yoffs)
{
  if (IntrfProcsRec.pSetIndexSymbol != NULL)
    IntrfProcsRec.SetIndexSymbol(IntrfProcsRec.pSetIndexSymbol, index, symbol, xoffs, yoffs);
}

//-----Currency name-----------------------------------------------------------
PChar Symbol()
{
  return IntrfProcsRec.Symbol(IntrfProcsRec.pSymbol);
}

//-----Currency digits after point---------------------------------------------
int Digits()
{
  return IntrfProcsRec.Digits(IntrfProcsRec.pDigits);
}

//-----Minimal currency point--------------------------------------------------
double Point()
{
  return IntrfProcsRec.Point(IntrfProcsRec.pPoint);
}

//-----Get open value----------------------------------------------------------
double iOpen(PChar Symbol, int TimeFrame, int index)
{
  return IntrfProcsRec.iOpen(IntrfProcsRec.p_iOpen, Symbol, TimeFrame, index);
}

//-----Get close value---------------------------------------------------------
double iClose(PChar Symbol, int TimeFrame, int index)
{
  return IntrfProcsRec.iClose(IntrfProcsRec.p_iClose, Symbol, TimeFrame, index);
}

//-----Get high value----------------------------------------------------------
double iHigh(PChar Symbol, int TimeFrame, int index)
{
  return IntrfProcsRec.iHigh(IntrfProcsRec.p_iHigh, Symbol, TimeFrame, index);
}

//-----Get low value-----------------------------------------------------------
double iLow(PChar Symbol, int TimeFrame, int index)
{
  return IntrfProcsRec.iLow(IntrfProcsRec.p_iLow, Symbol, TimeFrame, index);
}

//-----Get volume--------------------------------------------------------------
double iVolume(PChar Symbol, int TimeFrame, int index)
{
  return IntrfProcsRec.iVolume(IntrfProcsRec.p_iVolume, Symbol, TimeFrame, index);
}

//-----Get time of bar---------------------------------------------------------
TDateTime iTime(PChar Symbol, int TimeFrame, int index)
{
  return IntrfProcsRec.iTime(IntrfProcsRec.p_iTime, Symbol, TimeFrame, index);
}

//-----Get number of bars------------------------------------------------------
int iBars(PChar Symbol, int TimeFrame)
{
  return IntrfProcsRec.iBars(IntrfProcsRec.p_iBars, Symbol, TimeFrame);
}

//-----Get open value in default currency and timeframe------------------------
double Open(int index)
{
  return IntrfProcsRec.Open(IntrfProcsRec.pOpen, index);
}

//-----Get close value in default currency and timeframe-----------------------
double Close(int index)
{
  return IntrfProcsRec.Close(IntrfProcsRec.pClose, index);
}

//-----Get high value in default currency and timeframe------------------------
double High(int index)
{
  return IntrfProcsRec.High(IntrfProcsRec.pHigh, index);
}

//-----Get low value in default currency and timeframe-------------------------
double Low(int index)
{
  return IntrfProcsRec.Low(IntrfProcsRec.pLow, index);
}

//-----Get volume in default currency and timeframe----------------------------
double Volume(int index)
{
  return IntrfProcsRec.Volume(IntrfProcsRec.pVolume, index);
}

//-----Get time of bar in default currency and timeframe-----------------------
TDateTime Time(int index)
{
  return IntrfProcsRec.Time(IntrfProcsRec.pTime, index);
}

//-----Get number of bars in default currency and timeframe--------------------
int Bars()
{
  return IntrfProcsRec.Bars(IntrfProcsRec.pBars);
}

//-----Set indicator output window---------------------------------------------
void SetOutputWindow(TOutputWindow aWindow)
{
  IntrfProcsRec.SetOutputWindow(IntrfProcsRec.pSetOutputWindow, aWindow);
}

//-----Set fixed maximum/minimum for indicator window--------------------------
void SetFixedMinMaxValues(double aMin, double aMax)
{
  IntrfProcsRec.SetFixedMinMaxValues(IntrfProcsRec.pSetFixedMinMaxValues, aMin, aMax);
}

//-----Add level line to indicator window--------------------------------------
void AddLevel(double value, TPenStyle style, int width, TColor color)
{
  IntrfProcsRec.AddLevel(IntrfProcsRec.pAddLevel, value, style, width, color);
}

//-----Set empty value that will not be painted--------------------------------
void SetEmptyValue(double value)
{
  IntrfProcsRec.SetEmptyValue(IntrfProcsRec.pSetEmptyValue, value);
}

//-----Set indicator precision-------------------------------------------------
void IndicatorDigits(int digits)
{
  if (IntrfProcsRec.pIndicatorDigits != NULL)
	IntrfProcsRec.IndicatorDigits(IntrfProcsRec.pIndicatorDigits, digits);
}

//-----Get indicator timeframe-------------------------------------------------
int Timeframe()
{
  if (IntrfProcsRec.pTimeframe != NULL)
	return int(IntrfProcsRec.Timeframe(IntrfProcsRec.pTimeframe));
  else
	return PERIOD_M1;
}

//-----Set index label---------------------------------------------------------
void SetIndexLabel(int index, PChar text)
{
  if (IntrfProcsRec.SetIndexLabel != NULL)
    IntrfProcsRec.SetIndexLabel(IntrfProcsRec.pSetIndexLabel, index, text);
}

void SetIndexLabel(int index, string s){
	char* dstP;
	dstP = new char[ s.length()+1 ];
	//strcpy(dstP, s.c_str());
	strcpy_s( dstP, s.length()+1, s.c_str() );
	SetIndexLabel(index, dstP);
	delete[] dstP;
}

void SetIndexLabel(int index, stringstream& s){
	SetIndexLabel(index, s.str());
}
//-----Create object-----------------------------------------------------------
bool ObjectCreate(PChar name, TObjectType ObjType, int window,
  TDateTime time1, double price1, TDateTime time2, double price2, 
  TDateTime time3, double price3)
{
  if (IntrfProcsRec.pObjectCreate != NULL) 
    return IntrfProcsRec.ObjectCreate(IntrfProcsRec.pObjectCreate, name, ObjType, window, 
      time1, price1, time2, price2, time3, price3);
  
  return false;
}

//-----Delete object by name---------------------------------------------------
void ObjectDelete(PChar name)
{
  if (IntrfProcsRec.pObjectDelete != NULL)
    IntrfProcsRec.ObjectDelete(IntrfProcsRec.pObjectDelete, name);
}

//-----Check if object already exists------------------------------------------
bool ObjectExists(PChar name)
{
  if (IntrfProcsRec.pObjectExists != NULL)
    return IntrfProcsRec.ObjectExists(IntrfProcsRec.pObjectExists, name);
  else
    return false;
}

//-----Get object type---------------------------------------------------------
TObjectType ObjectType(PChar name)
{
  if (IntrfProcsRec.pObjectType != NULL)
    return TObjectType(IntrfProcsRec.ObjectType(IntrfProcsRec.pObjectType, name));
  else
    return obj_Text;
}

bool ObjectSet(PChar name, int index, double value)
{
  if (IntrfProcsRec.pObjectSet != NULL)
    return IntrfProcsRec.ObjectSet(IntrfProcsRec.pObjectSet, name, index, value);
  else 
    return false;
}

//-----Get object property-----------------------------------------------------
double ObjectGet(PChar name, int index)
{
  if (IntrfProcsRec.pObjectGet != NULL)
    return IntrfProcsRec.ObjectGet(IntrfProcsRec.pObjectGet, name, index);
  else
    return 0;
}

//-----Delete all objects------------------------------------------------------
void ObjectsDeleteAll(int window, TObjectType ObjType)
{
  if (IntrfProcsRec.pObjectsDeleteAll != NULL)
    IntrfProcsRec.ObjectsDeleteAll(IntrfProcsRec.pObjectsDeleteAll, window, ObjType);
}

//-----Get interface version---------------------------------------------------
void GetInterfaceVersion(int& MajorValue, int& MinorValue)
{
  if (IntrfProcsRec.pGetInterfaceVersion != NULL)
    IntrfProcsRec.GetInterfaceVersion(IntrfProcsRec.pGetInterfaceVersion, MajorValue, MinorValue);
  else
    {
      MajorValue = 1;
      MinorValue = 3;
    }
}

//-----Set text----------------------------------------------------------------
bool ObjectSetText(PChar name, PChar text, int FontSize, PChar FontName, TColor Color)
{
  if (IntrfProcsRec.pObjectSetText != NULL)
    return IntrfProcsRec.ObjectSetText(IntrfProcsRec.pObjectSetText, name, text, 
      FontSize, FontName, Color);
  else
    return false;
}

bool ObjectSetText(PChar name, string text, int FontSize, PChar FontName, TColor Color)
{
 	char* dstP;
	dstP = new char[ text.length()+1 ];
	strcpy_s(dstP, text.length()+1, text.c_str());
	bool b = ObjectSetText(name, dstP, FontSize, FontName, Color);
	delete[] dstP;
	return b;
}

bool ObjectSetText(PChar name, stringstream& text, int FontSize, PChar FontName, TColor Color){
		return ObjectSetText(name, text.str(), FontSize, FontName, Color);
}
	
bool ObjectSetText(PChar name, int i, int FontSize, PChar FontName, TColor Color)
{
		std::stringstream s;
		s << i;
		return ObjectSetText(name, s.str(), FontSize, FontName, Color);
}

bool ObjectSetText(PChar name, double d, int FontSize, PChar FontName, TColor Color)
{
		std::stringstream s;
		s << d;
		return ObjectSetText(name, s.str(), FontSize, FontName, Color);
}

//-----Get text---------------------------------------------------------------
PChar ObjectGetText(PChar name)
{
  if (IntrfProcsRec.ObjectGetText != NULL)
    return(IntrfProcsRec.ObjectGetText(IntrfProcsRec.pObjectGetText, name));
  else
    return(NULL);
}

//-----Set buffer shift--------------------------------------------------------
void SetBufferShift(int index, int Shift)
{
  if (IntrfProcsRec.SetBufferShift != NULL)
	IntrfProcsRec.SetBufferShift(IntrfProcsRec.pSetBufferShift, index, Shift);
}

//-----Get buffer value-------------------------------------------------------
double GetIndicatorBufferValue(TIndicatorOption indicator, int index)
{
  if ((IntrfProcsRec.GetIndicatorBufferValue != NULL) &&
      (indicator.BufferPtr != NULL))
	  return IntrfProcsRec.GetIndicatorBufferValue(IntrfProcsRec.pGetIndicatorBufferValue, indicator, index);
  else
    return(0);
}

//-----Get number of graphical objects on associated chart--------------------
int ObjectsTotal()
{
  if (IntrfProcsRec.ObjectsTotal != NULL) 
    return IntrfProcsRec.ObjectsTotal(IntrfProcsRec.pObjectsTotal);
  else
    return(0);
}

//-----Get object's name by its index-----------------------------------------
PChar ObjectName(int index)
{
  if (IntrfProcsRec.ObjectName != NULL) 
    return (IntrfProcsRec.ObjectName(IntrfProcsRec.pObjectName, index));
  else
    return(NULL);
}

//-----Get bar shift by its time----------------------------------------------
int iBarShift(PChar Symbol, int TimeFrame, TDateTime time, bool Exact)
{
  if (IntrfProcsRec.iBarShift == NULL) return -1;
  return IntrfProcsRec.iBarShift(IntrfProcsRec.piBarShift, Symbol, TimeFrame, time, Exact);
}

//-----Get highest value in array---------------------------------------------
int iHighest(PChar Symbol, int TimeFrame, int _type, int count, int index)
{
  if (IntrfProcsRec.iHighest == NULL) return -1;
  return IntrfProcsRec.iHighest(IntrfProcsRec.piHighest, Symbol, TimeFrame, _type, count, index);
}

//-----Get lowest value in array----------------------------------------------
int iLowest(PChar Symbol, int TimeFrame, int _type, int count, int index)
{
  if (IntrfProcsRec.iLowest == NULL) return -1;
  return IntrfProcsRec.iLowest(IntrfProcsRec.piLowest, Symbol, TimeFrame, _type, count, index);
}

//-----Convert chart index to screen x coordinate-----------------------------
int ChartToScrX(int index)
{
  if (IntrfProcsRec.ChartToScrX == NULL) return 0;
  return IntrfProcsRec.ChartToScrX(IntrfProcsRec.pChartToScrX, index);
}

//-----Convert chart price to screen y coordinate-----------------------------
int ChartToScrY(double price)
{
  if (IntrfProcsRec.ChartToScrY == NULL) return 0;
  return IntrfProcsRec.ChartToScrY(IntrfProcsRec.pChartToScrY, price);
}

//-----Convert screen x coordinate to chart bar's index-----------------------
int ScrToChartX(int x)
{
  if (IntrfProcsRec.ScrToChartX == NULL) return 0;
  return IntrfProcsRec.ScrToChartX(IntrfProcsRec.pScrToChartX, x);
}

//-----Convert screen y coordinate to chart price-----------------------------
double ScrToChartY(int y)
{
  if (IntrfProcsRec.ScrToChartY == NULL) return 0.0;
  return IntrfProcsRec.ScrToChartY(IntrfProcsRec.pScrToChartY, y);
}

//-----Get information about chart--------------------------------------------
bool  GetChartInfo(TChartInfo& ChartInfo)
{
  if (IntrfProcsRec.GetChartInfo == NULL) return false;
  
  ChartInfo.dwSize = sizeof(ChartInfo);
  return IntrfProcsRec.GetChartInfo(IntrfProcsRec.pGetChartInfo, ChartInfo);
}

//-----Get current bid price--------------------------------------------------
double Bid()
{
  if (IntrfProcsRec.Bid == NULL) return 0;
  return IntrfProcsRec.Bid(IntrfProcsRec.pBid);
}

//-----Get current bid price--------------------------------------------------
double Ask()
{
  if (IntrfProcsRec.Ask == NULL) return 0;
  return IntrfProcsRec.Ask(IntrfProcsRec.pAsk);
}

//-----Get current time-------------------------------------------------------
TDateTime TimeCurrent()
{
  if (IntrfProcsRec.TimeCurrent == NULL) return 0;
  return IntrfProcsRec.TimeCurrent(IntrfProcsRec.pTimeCurrent);
}

void SetCalcModeMT4()
{
  if (IntrfProcsRec.SetCalcModeMT4 == NULL) return;
  IntrfProcsRec.SetCalcModeMT4(IntrfProcsRec.pSetCalcModeMT4);
}

int mql4_IndicatorCounted()
{
	if (IntrfProcsRec.mql4_IndicatorCounted == NULL) return 0;
	return IntrfProcsRec.mql4_IndicatorCounted(IntrfProcsRec.pmql4_IndicatorCounted);
}
