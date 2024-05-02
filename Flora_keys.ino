#include "MIDIUSB.h"
#include "PitchToNote.h"

const int triggerPin = 3;
const int echoPin = 12; // Echo pin for the sensor
const byte notePitch = 60; // MIDI note C4

const int maxDistance = 400; // Maximum distance in cm
const int thresholdDistance = 400; // Distance threshold to trigger notes

void setup() {
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  triggerSensor();
  int distance = getDistance(echoPin);
  sendSensorData(distance);
  delay(5); // Adjust delay as needed
}

void triggerSensor() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
}

int getDistance(int echoPin) {
  long duration;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Calculate distance in cm
  return distance;
}

void sendSensorData(int distance) {
  if (distance > 0 && distance < thresholdDistance) {
    int velocity = map(distance, 0, thresholdDistance, 127, 0); // Map distance to velocity
    noteOn(0, notePitch, velocity);
  } else {
    noteOff(0, notePitch, 0);
  }
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
