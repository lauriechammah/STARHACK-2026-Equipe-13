# STARHACK-2026-Equipe-13

Smart MedBox
Inspiration
L'idée de la Smart MedBox est née d'un constat personnel : voir nos grands-parents lutter contre l'oubli de leurs médicaments. Cette situation crée une inquiétude constante pour les familles. Nous avons voulu offrir une solution concrète aux personnes atteintes d'Alzheimer ou à celles qui ne sont pas à l'aise avec les technologies complexes. Notre objectif était de créer un dispositif simple, efficace et rassurant.

Fonctionnement
La Smart MedBox est un boîtier intelligent qui contient les médicaments et rappelle au patient l'heure exacte de sa prise grâce à un système d'alertes visuelles et sonores.

Programmation simplifiée : L'utilisateur règle l'heure de l'alarme via une télécommande infrarouge (format HHMM).

Alerte intelligente : À l'heure dite, la boîte émet un signal sonore et la LED clignote en rouge.

Arrêt naturel : L'alarme s'arrête automatiquement dès que le patient ouvre la boîte pour prendre ses médicaments, grâce à un capteur de lumière (LDR).

Mode quotidien : Une fois la prise faite, le système se remet en veille pour le lendemain sans aucune manipulation supplémentaire.

Méthode de fabrication
Nous avons construit ce projet intégralement avec une forte dimension écologique :

Matériaux recyclés : Le châssis principal provient de pièces d'impression 3D initialement destinées au rebut. Nous avons également utilisé du carton mousse et du carton récupéré pour la structure.

Électronique : Le système est piloté par un Arduino Uno, relié à un module RTC (Real Time Clock) pour la précision de l'heure, un récepteur infrarouge, un buzzer et un capteur de lumière (LDR).

Code : Utilisation d'un algorithme non-bloquant pour gérer simultanément le temps et la lecture des signaux infrarouges.

Configuration technique (Pins)
Composant	Broche Arduino
Récepteur IR	A0
Capteur de lumière (LDR)	A1
Buzzer	A2
LED Rouge	A3
LED Verte	10
LED Bleue	11
Défis rencontrés
Le plus gros défi a été un changement de direction radical. Initialement, nous souhaitions intégrer un capteur de rythme cardiaque, mais celui-ci n'était pas disponible. Nous avons dû repartir de zéro en un temps record. Sur le plan technique, nous avons optimisé le code pour éviter les fonctions bloquantes, permettant au buzzer de sonner sans interrompre la surveillance du système.

Accomplissements dont nous sommes fiers
En tant qu'étudiants de première année, nous avons réussi à livrer un projet entièrement fonctionnel. Passer d'une idée sur papier à un prototype capable de gérer le temps réel et de réagir à une télécommande est une grande victoire pour nous.

Apprentissages
Ce projet a été un véritable accélérateur d'apprentissage :

Matériel : Réalisation de montages sur plaque d'essai et application de la loi d'Ohm :

Débogage : Apprentissage de la traque d'erreurs logicielles via le moniteur série.

Prochaines étapes
Optimisation : Réduire la taille du boîtier pour le rendre plus compact.

Conception : Mieux isoler les composants électroniques pour plus de durabilité.

Capacité : Ajouter des compartiments séparés pour les prises du matin, midi et soir.

Connectivité : Ajouter un module de notification pour prévenir les proches en cas d'oubli prolongé.
