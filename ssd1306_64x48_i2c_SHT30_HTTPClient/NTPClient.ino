extern unsigned long long ullEpochTime;

void NTPClient_Setup()
{
  timeClient.begin();

  // Update one during setup
  timeClient.update();
  ullEpochTime = timeClient.getEpochTime();
  ullMSecTicks = 0; // reset it

  Serial.print("ullEpochTime : "); SerialPrintULL(ullEpochTime);
}
