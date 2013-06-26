
#ifndef TestBuilder_H
#define TestBuilder_H

#include <stdio.h>
#include "ECGPkh.h"
//#include "ArrythmiaDetector.h"
//#include "ECG_Module.h"
#include "HistogramDisplay.h"
//#include "QRSDetector.h"
#include "TMDQueue.h"
//#include "WaveformDisplay.h"

struct ArrytmiaDetector
{
   int Id;
};

struct ECG_Module
{
   int Id;
};

struct QRSDetector
{
   int Id;
};

struct WaveformDisplay
{
   int Id;
};

typedef struct TestBuilder TestBuilder;

struct TestBuilder
{
	struct ArrytmiaDetector itsArrytmiaDetector;
	struct ECG_Module itsECG_Module;
	struct HistogramDisplay itsHistogramDisplay;
	struct QRSDetector itsQRSDetector;
	struct TMDQueue itsTMDQueue;
	struct WaveformDisplay itsWaveFromDisplay;
};

void TestBuilder_Init(TestBuilder* const me);
void TestBuilder_Cleanup(TestBuilder* const me);
struct ArrytmiaDetector* TestBuilder_getItsArrytmiaDetector(const TestBuilder* const me);
struct ECG_Module* TestBuilder_getItsECG_Module(const TestBuilder* const me);
struct HistogramDisplay* TestBuilder_getItsHistogramDisplay(const TestBuilder* const me);
struct QRSDetector* TestBuilder_getItsQRSDetector(const TestBuilder* const me);
struct TMDQueue* TestBuilder_getItsTMDQueue(const TestBuilder* const me);
struct WaveformDisplay* TestBuilder_getItsWaveformDisplay(const TestBuilder* const me);
TestBuilder* TestBuilder_Create(void);
void TestBuilder_Destroy(TestBuilder* const me);

#endif