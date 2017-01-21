#include <windows.h>
#include "Entete.h"
#pragma comment (lib,"DescenteDLL.lib")  
//%%%%%%%%%%%%%%%%%%%%%%%%% IMPORTANT: %%%%%%%%%%%%%%%%%%%%%%%%% 
//Le fichier de probleme (.txt) et les fichiers de la DLL (DescenteDLL.dll et DescenteDLL.lib) doivent 
//se trouver dans le r�pertoire courant du projet pour une ex�cution � l'aide du compilateur. Indiquer les
//arguments du programme dans les propri�t�s du projet - d�bogage - arguements.
//Sinon, utiliser le r�pertoire execution.

//*****************************************************************************************
// Prototype des fonctions se trouvant dans la DLL 
//*****************************************************************************************
//DESCRIPTION:	Lecture du Fichier probleme et initialiation de la structure Problem
extern "C" _declspec(dllimport) void LectureProbleme(std::string FileName, TProblem & unProb, TAlgo &unAlgo);

//DESCRIPTION:	Fonction d'affichage � l'�cran permettant de voir si les donn�es du fichier probl�me ont �t� lues correctement
extern "C" _declspec(dllimport) void AfficherProbleme (TProblem unProb);

//DESCRIPTION: Affichage d'une solution a l'�cran pour validation (avec ou sans d�tails des calculs)
extern "C" _declspec(dllimport) void AfficherSolution(const TSolution uneSolution, TProblem unProb, std::string Titre, bool AvecCalcul);

//DESCRIPTION: Affichage � l'�cran de la solution finale, du nombre d'�valuations effectu�es et de certains param�tres
extern "C" _declspec(dllimport) void AfficherResultats (const TSolution uneSol, TProblem unProb, TAlgo unAlgo);

//DESCRIPTION: Affichage dans un fichier(en append) de la solution finale, du nombre d'�valuations effectu�es et de certains param�tres
extern "C" _declspec(dllimport) void AfficherResultatsFichier (const TSolution uneSol, TProblem unProb, TAlgo unAlgo, std::string FileName);

//DESCRIPTION:	�valuation de la fonction objectif d'une solution et MAJ du compteur d'�valuation. 
//				Fonction objectif repr�sente le retard total pond�r�
extern "C" _declspec(dllimport) void EvaluerSolution(TSolution & uneSol, TProblem unProb, TAlgo &unAlgo);

//DESCRIPTION:	Cr�ation d'une s�quence al�atoire de parcours des villes et �valuation de la fonction objectif. Allocation dynamique de m�moire
// pour la s�quence (.Seq)
extern "C" _declspec(dllimport) void CreerSolutionAleatoire(TSolution & uneSolution, TProblem unProb, TAlgo &unAlgo);

//DESCRIPTION: Copie de la s�quence et de la fonction objectif dans une nouvelle TSolution. La nouvelle TSolution est retourn�e.
extern "C" _declspec(dllimport) void CopierSolution (const TSolution uneSol, TSolution &Copie, TProblem unProb);

//DESCRIPTION:	Lib�ration de la m�moire allou�e dynamiquement
extern "C" _declspec(dllimport) void	LibererMemoireFinPgm	(TSolution uneCourante, TSolution uneNext, TSolution uneBest, TProblem unProb);

//*****************************************************************************************
// Prototype des fonctions locales 
//*****************************************************************************************

//DESCRIPTION:	Cr�ation d'une solution voisine � partir de la solution uneSol. NB:uneSol ne doit pas �tre modifi�e
TSolution GetSolutionVoisine (const TSolution uneSol, TProblem unProb, TAlgo &unAlgo);

//DESCRIPTION:	 Echange de deux villes s�lectionn�e al�atoirement. NB:uneSol ne doit pas �tre modifi�e
TSolution	Echange			(const TSolution uneSol, TProblem unProb, TAlgo &unAlgo);


//******************************************************************************************
// Fonction main
//*****************************************************************************************
int main(int NbParam, char *Param[])
{
	TSolution Courante;		//Solution active au cours des it�rations
	TSolution Next;			//Solution voisine retenue � une it�ration
	TSolution Best;			//Meilleure solution depuis le d�but de l'algorithme //Non utilis� pour le moment 
	TProblem LeProb;		//D�finition de l'instance de probl�me
	TAlgo LAlgo;			//D�finition des param�tres de l'agorithme
	string NomFichier;
		
	//**Lecture des param�tres
	NomFichier.assign(Param[1]);
	LAlgo.NB_EVAL_MAX= atoi(Param[2]);

	//**Lecture du fichier de donnees
	LectureProbleme(NomFichier, LeProb, LAlgo);
	AfficherProbleme(LeProb);
	
	//**Cr�ation de la solution initiale 
	CreerSolutionAleatoire(Courante, LeProb, LAlgo);
	AfficherSolution(Courante, LeProb, "SolInitiale: ", false);

	do
	{
		Next = GetSolutionVoisine(Courante, LeProb, LAlgo);
		//AfficherSolution(Courante, LeProb, "Courante: ", false);
		//AfficherSolution(Next, LeProb, "Next: ", false);
		if (Next.FctObj <= Courante.FctObj)	//**am�lioration
		{
				Courante = Next;
				cout << "Fct Obj Nouvelle Courante: " << Courante.FctObj << endl;
				//AfficherSolution(Courante, LeProb, "NouvelleCourante: ", false);
		}
	}while (LAlgo.CptEval < LAlgo.NB_EVAL_MAX && Courante.FctObj!=0);

	AfficherResultats(Courante, LeProb, LAlgo);
	AfficherResultatsFichier(Courante, LeProb, LAlgo,"Resultats.txt");
	
	LibererMemoireFinPgm(Courante, Next, Best, LeProb);

	system("PAUSE");
	return 0;
}

//DESCRIPTION: Cr�ation d'une solution voisine � partir de la solution uneSol qui ne doit pas �tre modifi�e.
//Dans cette fonction, on appel le TYPE DE VOISINAGE s�lectionn� + on d�termine la STRAT�GIE D'ORIENTATION. 
//Ainsi, si la R�GLE DE PIVOT n�cessite l'�tude de plusieurs voisins, la fonction TYPE DE VOISINAGE sera appel�e plusieurs fois.
//Le TYPE DE PARCOURS dans le voisinage interviendra normalement dans la fonction TYPE DE VOISINAGE.
TSolution GetSolutionVoisine (const TSolution uneSol, TProblem unProb, TAlgo &unAlgo)
{
	//Type de voisinage : � MODIFIER (Echange 2 commandes al�atoires)
	//Parcours dans le voisinage : � MODIFIER	(Aleatoire)
	//R�gle de pivot : � MODIFIER	(First-Impove)

	//TSolution unVoisin;
	//unVoisin = Echange(uneSol, unProb, unAlgo);
	//return (unVoisin);

	////////////////////////////////////////////////////////////////
	//Etablissons la liste de voisins g�n�r�s
	int k = 10;
	std::vector<TSolution> listeVoisins(k);
	std::vector<TSolution> listeVoisinsMeilleurs(k);
	int compteurVoisinsMeilleurs;
	do 
	{
		listeVoisinsMeilleurs.clear();
		compteurVoisinsMeilleurs = 0;
		for (int i = 0; i<k; i++)
		{
			listeVoisins[i] = Echange(uneSol, unProb, unAlgo);
			//Avantageux?
			if (listeVoisins[i].RetardP<uneSol.RetardP)
			{
				compteurVoisinsMeilleurs++;
				listeVoisinsMeilleurs.push_back(listeVoisins[i]);
			}
		}
		//On boucle tant qu'il n'y a pas d'am�lioration
	} while (compteurVoisinsMeilleurs==0);

	//Pr�levons au hasard parmis les voisins avantageux
	int pos = rand() % compteurVoisinsMeilleurs;
	
	return (listeVoisinsMeilleurs[pos]);
	/////////////////////////////////////////////////////////////////
	
}

//DESCRIPTION: Echange de deux commandes s�lectionn�es al�atoirement
TSolution Echange (const TSolution uneSol, TProblem unProb, TAlgo &unAlgo)
{
	TSolution Copie;
	int PosA, PosB, Tmp;

	//Utilisation d'une nouvelle TSolution pour ne pas modifier La solution courante (uneSol)
	CopierSolution(uneSol, Copie, unProb);
	
	//Tirage al�atoire des 2 commandes
	PosA = rand() % unProb.NbCom;
	do
	{
		PosB = rand() % unProb.NbCom;
	}while (PosA == PosB); //Validation pour ne pas consommer une �valuation inutilement
	
	//Echange des 2 commandes
	Tmp = Copie.Seq[PosA];
	Copie.Seq[PosA] = Copie.Seq[PosB];
	Copie.Seq[PosB] = Tmp;
	
	//Le nouveau voisin doit �tre �valu� 
	EvaluerSolution(Copie, unProb, unAlgo);
	return(Copie);
}