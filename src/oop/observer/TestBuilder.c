
#include "TestBuilder.h"

static void initRelations(TestBuilder* const me);
static void cleanUpRelations(TestBuilder* const me);

void TestBuilder_Init(TestBuilder* const me)
{
   initRelations(me);
}

void TestBuilder_Cleanup(TestBuilder* const me)
{
   cleanUpRelations(me);
}

struct ArrytmiaDetector* TestBuilder_getItsArrytmiaDetector(const TestBuilder* const me)
{
   return (struct ArrytmiaDetector*)&(me->itsArrythmiaDetector);  
}

struct ECG_Module* TestBuilder_getItsECG_Module(const TestBuilder* const me)
{
   return (struct ECG_Module*)&(me->itsECG_Module);
}

struct HistogramDisplay* TestBuilder_getItsHistogramDisplay(const TestBuilder* const me)
{
   return (struct HistogramDisplay*)&(me->itsHistogramDisplay);
}

struct QRSDetector* TestBuilder_getItsQRSDetector(const TestBuilder* const me)
{
   return (struct QRSDetector*)&(me->itsQRSDetector);
}

struct TMDQueue* TestBuilder_getItsTMDQueue(const TestBuilder* const me)
{
   return (struct TMDQueue*)&(me->itsTMDQueue);
}

struct WaveformDisplay* TestBuilder_getItsWaveformDisplay(const TestBuilder* const me)
{
   return (struct WaveformDisplay*)&(me->itsWaveformDisplay);
}

TestBuilder* TestBuilder_Create(void)
{
   TestBuilder* me = (TestBuilder*)malloc(sizeof(TestBuilder));
   if (me != NULL)
   {
      TestBuilder_Init(me);
   }
   return me;
}

void TestBuilder_Destroy(TestBuilder* const me)
{
   if (me != NULL)
   {
      TestBuilder_Cleanup(me);
   }
   free(me);
}

static void initRelations(TestBuilder* const me)
{
   ArrytmiaDetector_Init(&(me->itsArrytmiaDetector));
   ECG_Module_Init(&(me->itsECG_Module));
   HistogramDisplay_Init(&(me->itsHistogramDisplay));
   QRSDetector_Init(&(me->itsQRSDetector));
   TMDQueue_Init(&(me->itsTMDQueue));
   WaveformDisplay_Init(&(me->itsWaveFromDisplay));

   ECG_Module_setItsTMDQueue(&(me->itsECG_Module), &(me->itsTMDQueue));
   HistogramDisplay_setItsTMDQueue(&(itsQRSDetector), &(me->itsTMDQueue));
   
   
   ArrytmiaDetector_Init(&(me->itsArrytmiaDetector))

   
}