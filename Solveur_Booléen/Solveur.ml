(*	MARICIC ALEXANDRE 11803731
		CHEN KEVIN 11807217
*)

type eb = V of int | VRAI | FAUX | AND of eb * eb | OR of eb * eb | XOR of eb * eb | NAND of eb * eb | NOT of eb ;;

	(*Définition des fonctions logique*)
let et e1 e2 =
  if e1 = VRAI then e2
  else FAUX;;

let ou e1 e2 =
  if e1 = VRAI then VRAI
  else e2;;

let non e1 =
  if e1 = FAUX then VRAI
  else FAUX;;

let xor e1 e2 =
  if e1 = e2 then FAUX
  else VRAI;;

let nonand e1 e2 =
  if e1 = FAUX then VRAI else
  if e2 = VRAI then FAUX else VRAI;;

	(*Recherche d'un élément dans une liste*)
let rec contient e l =
  match l with
  |[] -> false
  |h::t ->
      if h = e then true
      else contient e t;;

	(*Ajout d'un élément dans une liste*)
let rec ajouter e l =
  match l with
  |[] -> []
  |[[]] -> [[e]]
  |h::t -> (e::h)::(ajouter e t);;

	(*Retire les doublons d'une liste*)
let rec unique l =
  match l with
  |[] -> []
  |h::t -> if contient h t then unique t
      else h::unique t;;

	(*Récupération des variables dans les fonctions logiques*)
let rec variables l =
  match l with
  |AND(e1, e2) -> unique(((variables e1) @ (variables e2)))
  |OR(e1, e2) -> unique(((variables e1) @ (variables e2)))
  |XOR(e1, e2) -> unique(((variables e1) @ (variables e2)))
  |NOT(e1) -> unique((variables e1))
  |NAND(e1, e2) -> unique(((variables e1) @ (variables e2)))
  |V(x) -> unique(l::[])
  |_ -> [];;

	(*Récupération des variables dans les équations*)
let rec ensembles lg ld =
  match (lg, ld) with
  |([],[]) -> []
  |(hg::tg, hd::td) -> (variables hg) @ (variables hd) @ (ensembles tg td)
  |_ -> [];;

	(*Découpage de la liste avec distinction d*)
let rec decoupe l =
  match l with
  |[]-> [[]]
  |h::t -> let dec = decoupe t in let plus = ajouter h dec in plus @ dec;;

	(*Attribution des valeur VRAI/FAUX*)
let rec total var envr =
  match var with
  |[] -> []
  |h::tvar -> if contient h envr
      then (h,VRAI)::(total tvar envr)
      else (h,FAUX)::(total tvar envr);;

	(*Création de la table de vérité*)
let rec environnement var lvar =
  match lvar with
  |[] -> []
  |hvar::tvar -> (total var hvar)::(environnement var tvar);;

	(*Affectation de valeur de vérité*)
let rec reel eb vf =
  match vf with
  |[] -> FAUX
  |(heb,valeb)::t -> if heb = eb then valeb else reel eb t;;

	(*Évaluation de fonctions logiques*)
let rec eval el =
  match el with
  |VRAI -> VRAI
  |FAUX -> FAUX
  |AND(e1, e2) -> et(eval e1) (eval e2)
  |OR(e1, e2) -> ou(eval e1) (eval e2)
  |XOR(e1, e2) -> xor(eval e1) (eval e2)
  |NOT(e1) -> non(eval e1)
  |NAND(e1, e2) -> nonand(eval e1) (eval e2)
  |_ -> el;;

	(*Remplacement des variables par des valeur VRAI/FAUX*)
let rec remplacer eb vf =
  match eb with
  |AND(e1, e2) -> AND(remplacer e1 vf,remplacer e2 vf)
  |OR(e1, e2) -> OR(remplacer e1 vf,remplacer e2 vf)
  |XOR(e1, e2) -> XOR(remplacer e1 vf,remplacer e2 vf)
  |NAND(e1, e2) -> NAND(remplacer e1 vf,remplacer e2 vf)
  |NOT(e1) -> NOT(remplacer e1 vf)
  |V(x) -> reel eb vf
  |VRAI -> VRAI
  |FAUX -> FAUX;;

	(*Vérification de l'égalité entre les membres gauches et droites*)
let rec egalite lebg lebd envivar =
  match (lebg,lebd) with
  |([],[]) -> true
  |([], _::_) -> true
  |(_::_, []) -> true
  |(hebg::tebg, hebd::tebd) -> if (eval (remplacer hebg envivar)) = (eval (remplacer hebd envivar))
      then (egalite tebg tebd envivar) else false;;

	(*Affectation des valeur VRAI/FAUX dans la table de vérité*)
let rec organizer ebvar ebvf envivar =
  match envivar with
  |[] -> []
  |hvar::tvar -> if egalite ebvar ebvf hvar = true
      then hvar::(organizer ebvar ebvf tvar)
      else organizer ebvar ebvf tvar;;

	(*Recherche de la solution
		Solveur Booléen*)
let solution equag equad = organizer equag equad (environnement (unique(ensembles equag equad)) (decoupe (unique(ensembles equag equad))));;

	(*Exemple du sujet*)
let v = [OR(V 1, V 2); XOR(V 1, V 3); NAND(V 1,(AND(V 2, V 3)))];;
let w = [VRAI; V 2; VRAI];;
let sol = solution v w;;

	(*Quelques autres exemples*)
let e = [NAND(OR(V 1, V 2), V 3); NOT(OR(V 1, V 3)); XOR(NAND(V 1, V 2), V 3)];;
let d = [VRAI; V 2; V 1];;
let sol2 = solution e d;;

let i = [AND(V 1, NOT(V 1))]
let j = [FAUX]
let k = [VRAI]
let solj = solution a j
let solk = solution a k

(*tautologie*)
let t = [OR(OR(NOT(V 1), V 2), V 1); OR(OR(NOT(V 3), V 2), V 3)];;
let u = [VRAI; VRAI];;
let sol3 = solution t u;;

(*Étape par étape*)
let a = unique(ensembles v w)
let b = decoupe a
let c = environnement a b
let d = organizer v w c
