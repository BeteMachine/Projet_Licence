all: Game_SortingRobot Checker_SortingRobot mainNaif mainCirculaire mainLDC mainAVL test_AVL

entree_sortie.o: entree_sortie.c entree_sortie.h
	gcc -c entree_sortie.c

Solution.o: Solution.c Solution.h entree_sortie.h
	gcc -c Solution.c

Grille.o: Grille.c Grille.h Solution.h
	gcc -c Grille.c

API_AffGrille.o: API_AffGrille.c API_AffGrille.h Grille.h
	gcc -c API_AffGrille.c

AlgoNaif.o: AlgoNaif.c AlgoNaif.h
	gcc -c AlgoNaif.c

Circulaire.o: Circulaire.c Circulaire.h
	gcc -c Circulaire.c

AlgoLDC.o: AlgoLDC.c AlgoLDC.h
	gcc -c AlgoLDC.c

AlgoAVL.o: AlgoAVL.c AlgoAVL.h
	gcc -c AlgoAVL.c

Game_SortingRobot.o: Game_SortingRobot.c Grille.h API_AffGrille.h Solution.h
	gcc -c Game_SortingRobot.c

Game_SortingRobot: Game_SortingRobot.o API_AffGrille.o Grille.o Solution.o entree_sortie.o
	gcc -o Game_SortingRobot Game_SortingRobot.o API_AffGrille.o Grille.o Solution.o entree_sortie.o -lSDL -lm

Checker_SortingRobot.o: Checker_SortingRobot.c Grille.h API_AffGrille.h Solution.h
	gcc -c Checker_SortingRobot.c -lm

Checker_SortingRobot: Checker_SortingRobot.o  API_AffGrille.o Grille.o Solution.o entree_sortie.o
	gcc -o Checker_SortingRobot Checker_SortingRobot.o  API_AffGrille.o Grille.o Solution.o entree_sortie.o -lSDL -lm

mainNaif.o: mainNaif.c AlgoNaif.h
	gcc -c mainNaif.c

mainCirculaire.o: mainCirculaire.c Circulaire.h
	gcc -c mainCirculaire.c

mainLDC.o: mainLDC.c AlgoLDC.h
	gcc -c mainLDC.c
	
mainAVL.o: mainAVL.c AlgoAVL.h
	gcc -c mainAVL.c

test_AVL.o: test_AVL.c AlgoAVL.h
	gcc -c test_AVL.c

mainNaif: mainNaif.o  AlgoNaif.o
	gcc -o mainNaif mainNaif.o AlgoNaif.o API_AffGrille.o Grille.o Solution.o entree_sortie.o -lSDL -lm

mainCirculaire: mainCirculaire.o Circulaire.o
	gcc -o mainCirculaire mainCirculaire.o AlgoNaif.o Circulaire.o API_AffGrille.o Grille.o Solution.o entree_sortie.o -lSDL -lm

mainLDC: mainLDC.o  AlgoLDC.o
	gcc -o mainLDC mainLDC.o AlgoLDC.o Circulaire.o AlgoNaif.o API_AffGrille.o Grille.o Solution.o entree_sortie.o -lSDL -lm

mainAVL: mainAVL.o  AlgoAVL.o
	gcc -o mainAVL mainAVL.o AlgoAVL.o AlgoLDC.o Circulaire.o AlgoNaif.o API_AffGrille.o Grille.o Solution.o entree_sortie.o -lSDL -lm

test_AVL: test_AVL.o  AlgoAVL.o
	gcc -o test_AVL test_AVL.o AlgoAVL.o AlgoLDC.o Circulaire.o AlgoNaif.o API_AffGrille.o Grille.o Solution.o entree_sortie.o -lSDL -lm

clean:
	rm -f *.o Game_SortingRobot Checker_SortingRobot
