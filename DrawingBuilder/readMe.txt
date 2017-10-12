- J'ai utilisé les bibliothèques SFML et Box2D (les dernières versions), il faudra donc installer ces bibliothèques pour faire tourner le jeu.

- Les contrôles et instruction du jeu sont les suivantes :
	- Pour générer une figure (donc un cercle, un triangle ou un rectangle), il y a 2 méthodes.
		- On peut soit appuyer sur les figures situer dans la vue du haut
		- Soit appuyer sur l'une des touches suivantes du clavier : A (pour génerer un rectangle), Z (pour un cercle) et E (pour un triangle). 
	  La figure ainsi généré sera positionner au centre de la fenêtre si la méthode de génération 1 a été utilisé, sinon au niveau de la position de la souris
	  sur la fenêtre.
	- Pour déplacer une figure il suffit de rester appuyer sur le CLICK GAUCHE de la souris en se plaçant sur une figure.
	- On peut modifier la taille de la figure en effectuant un SCROLL UP ou SCROLL DOWN de la souris
	- On peut réaliser une rotation droite de l'objet en appuyant sur la touche RIGHT du clavier ou une rotation gauche en appuyant sur la touche LEFT du clavier. 
	- On peut également supprimer une figure en appuyant sur la touche SUPPR sur celle-ci.

- Les figures générées sont soumis aux lois de la physique

- Pour gagner il faut générer les objets et les placer afin de réaliser le dessin du niveau puis selectionner les figures du dessin via un CLICK DROIT

Remarque : - Un manque de fluidité est à noter dans le deplacement des figures, il faut que je vois le problème au niveau de l'update du monde 2D
	   - Le générateur de niveau est à venir (mon idée étant de recréer un monde identique au jeu et de construire une figure puis selectionner les figures du dessin
	     et sauvegarder les paramètres du dessin dans un fichier XML qui servira d'identifiant du niveau).