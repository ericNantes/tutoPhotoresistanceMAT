#include <Arduino.h>

#define PIN_LED 3

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);
  delay(100);
}

void loop() {
  const uint16_t SEUIL_NUIT = 400;
  const uint16_t SEUIL_JOUR = 800;
  const uint32_t DELAI_JOUR = 5000;   // Durée minimale en millisecondes de détection du jour avant extinction
  const uint32_t DELAI_NUIT = 1000;   // Durée minimale de détection de la nuit avant allumage
  static uint32_t heureJour = 0;    // Heure de la dernière détection de jour
  static uint32_t heureNuit = 0;    // Heure de la détection du début de la nuit


  enum tEtats {JOUR, DEBUT_NUIT, NUIT, DEBUT_JOUR};
  static tEtats etat = JOUR;
  static tEtats etatSuivant = JOUR;

  // Prise en compte de l'état suivant
  etat = etatSuivant;
 
  // Lecture des entrées
  uint16_t luminosite = analogRead(A0);
  uint32_t _millis = millis();

  // Sorties
  switch(etat) {
    case JOUR:
      digitalWrite(PIN_LED, LOW);
      break;
    case DEBUT_NUIT:
      digitalWrite(PIN_LED, LOW);
      break;
    case NUIT:
      digitalWrite(PIN_LED, HIGH);
      break;
    case DEBUT_JOUR:
      digitalWrite(PIN_LED, HIGH);
      break;
  }

  // Calcul de l'état suivant de la machine à états
  switch(etat) {
    case JOUR:
      if (luminosite <= SEUIL_NUIT) {
        etatSuivant = DEBUT_NUIT;
        heureNuit = _millis;
      }
      break;
    case DEBUT_NUIT:
      if (luminosite > SEUIL_NUIT) etatSuivant = JOUR;
      else
        if ((_millis - heureNuit) >= DELAI_NUIT) {
          etatSuivant = NUIT;
        }
      break;
    case NUIT:
      if (luminosite >= SEUIL_JOUR) {
        etatSuivant = DEBUT_JOUR;
        heureJour = _millis;
      }
      break;
    case DEBUT_JOUR:
      if (luminosite < SEUIL_JOUR) etatSuivant = NUIT;
      if ((_millis - heureJour) >= DELAI_JOUR) {
        etatSuivant = JOUR;
      }
      break;
    default: etatSuivant = JOUR;
  }
}