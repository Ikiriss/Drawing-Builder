- J'ai utilis� les biblioth�ques SFML et Box2D (les derni�res versions), il faudra donc installer ces biblioth�ques pour faire tourner le jeu.

- Les contr�les et instruction du jeu sont les suivantes :
	- Pour g�n�rer une figure (donc un cercle, un triangle ou un rectangle), il y a 2 m�thodes.
		- On peut soit appuyer sur les figures situer dans la vue du haut
		- Soit appuyer sur l'une des touches suivantes du clavier : A (pour g�nerer un rectangle), Z (pour un cercle) et E (pour un triangle). 
	  La figure ainsi g�n�r� sera positionner au centre de la fen�tre si la m�thode de g�n�ration 1 a �t� utilis�, sinon au niveau de la position de la souris
	  sur la fen�tre.
	- Pour d�placer une figure il suffit de rester appuyer sur le CLICK GAUCHE de la souris en se pla�ant sur une figure.
	- On peut modifier la taille de la figure en effectuant un SCROLL UP ou SCROLL DOWN de la souris
	- On peut r�aliser une rotation droite de l'objet en appuyant sur la touche RIGHT du clavier ou une rotation gauche en appuyant sur la touche LEFT du clavier. 
	- On peut �galement supprimer une figure en appuyant sur la touche SUPPR sur celle-ci.

- Les figures g�n�r�es sont soumis aux lois de la physique

- Pour gagner il faut g�n�rer les objets et les placer afin de r�aliser le dessin du niveau puis selectionner les figures du dessin via un CLICK DROIT

Remarque : - Un manque de fluidit� est � noter dans le deplacement des figures, il faut que je vois le probl�me au niveau de l'update du monde 2D
	   - Le g�n�rateur de niveau est � venir (mon id�e �tant de recr�er un monde identique au jeu et de construire une figure puis selectionner les figures du dessin
	     et sauvegarder les param�tres du dessin dans un fichier XML qui servira d'identifiant du niveau).