/**
 * Vérifie si le patient prend ses médicaments à l'heure prévue.
 * @param heureCible L'heure de la prise (0-23)
 * @param minuteCible La minute de la prise (0-59)
 * @param estOuverte État du pilulier (donnée de ta photo-résistance LDR)
 */
void verifierConformite(int heureSaisie, bool estOuverte)
{
    // 0. On récupère l'heure réelle via le module RTC
    DateTime maintenant = rtc.now();

    // 1. Étape d'extraction
    // On récupère les heures et les minutes séparéments
    int heureCible = heureSaisie / 100;  // Donne les deux premiers chiffres
    int minuteCible = heureSaisie % 100; // Donne les deux derniers chiffres

    // 2. Conversion en minutes totales pour simplifier les calculs
    // Exemple : 12h51 devient (12 * 60) + 51 = 771 minutes
    long minutesActuelles = (maintenant.hour() * 60) + maintenant.minute();
    long minutesVisees = (heureCible * 60) + minuteCible;

    // Fenêtre de tolérance (en minutes) avant l'alarme critique
    int tolerance = 2;

    // --- LOGIQUE DE DÉCISION ---

    // CAS 1 : C'est l'heure exacte ou on est dans la fenêtre de tolérance
    if (minutesActuelles >= minutesVisees && minutesActuelles <= (minutesVisees + tolerance))
    {
        if (!estOuverte)
        {
            Serial.println("RAPPEL : Il est l'heure de prendre le traitement.");
            // On peut demander à Mariya de faire un petit bip discret ici
        }
        else
        {
            Serial.println("Médicament pris à temps. Bravo !");
            mariya_arreterAlarme(); // On coupe le son si la boîte est ouverte
        }
    }

    // CAS 2 : Le temps de tolérance est dépassé et la boîte est restée fermée
    else if (minutesActuelles > (minutesVisees + tolerance))
    {
        if (!estOuverte)
        {
            Serial.print("ALERTE CRITIQUE : Oubli de ");
            Serial.print(minutesActuelles - minutesVisees);
            Serial.println(" minutes !");

            mariya_declencherAlarmeForte(); // Alarme d'urgence pour Alzheimer
        }
        else
        {
            // Le patient a fini par ouvrir la boîte, on arrête tout
            mariya_arreterAlarme();
        }
    }
}