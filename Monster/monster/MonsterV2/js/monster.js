/** MARICIC Alexandre 11803731
    CHEN Kévin 11807217 **/
    
var name = "";
var maxLife = 42;
var money = 0;
var awake = true;

var runButton = document.getElementById("run");
var fightButton = document.getElementById("fight");
var workButton = document.getElementById("work");
var sleepButton = document.getElementById("sleep");
var eatButton = document.getElementById("eat");
var showButton = document.getElementById("show");
var newButton = document.getElementById("new");
var killButton = document.getElementById("kill");
var box = document.getElementById("actionbox");
var statut = document.getElementById("statut");
var lotoButton = document.getElementById("loto");
const INTERVALLE = 7000;
setInterval(actionauhasard, INTERVALLE);

function initMonstre(nom, vie, argent)
{
    name = nom;
    life = vie;
    money = argent;
}

function afficheMonstre()
{
    var message = "Nom: " + name + ", Vie: " + life + ", Argent: " + money + ", Réveillé: " + awake;
    console.log(message);
    logBoite(message);
}

function go()
{
    initMonstre("Michel", 20, 100);
    showButton.addEventListener("click", afficheMonstre);
    updateStatus();
}

window.addEventListener("load", go);

function logBoite(message)
{
    let mes = document.createElement("p");
    mes.textContent = message;
    box.insertBefore(mes, box.firstChild);
}

function updateStatus()
{
    const monstername = document.getElementById("monstername");
    const monsterpv = document.getElementById("pv");
    const monstermoney = document.getElementById("money");
    const monsterawake = document.getElementById("awake");
    monstername.innerHTML = name;
    monsterpv.innerHTML = "Vie: " + life;
    monstermoney.innerHTML = "Argent: " + money;
    monsterawake.innerHTML = "Réveillé: " + awake;
/** if(!enVie)
    {
        killButton.disabled = true;
        runButton.disabled = true;
        fightButton.disabled = true;
        workButton.disabled = true;
        eatButton.disabled = true;
        sleepButton.disabled = true;
    }
    else if(enVie)
    {
        killButton.disabled = false;
        runButton.disabled = false;
        fightButton.disabled = false;
        workButton.disabled = false;
        eatButton.disabled = false;
        sleepButton.disabled = false;
    }
    if(!awake)
    {
        eatButton.disabled = true;
        workButton.disabled = true;
        fightButton.disabled = true;
        runButton.disabled = true;
    }
    else
    {
        killButton.disabled = false;
        runButton.disabled = false;
        fightButton.disabled = false;
        workButton.disabled = false;
        eatButton.disabled = false;
        sleepButton.disabled = false;
    }**/
    if(life <= maxLife)
        updatehp();
    Victoire();
}

var enVie = true;

function courir()
{
    if(!enVie)
    {
        logBoite(name + " n'est plus... Il ne peut plus courir.");
        return;
    }
    if(!awake)
    {
        logBoite(name + " fait sa sièste et ne répond pas.");
        return;
    }
    if(life <= 1)
    {
        logBoite(name + " n'a pas assez de vie pour courir !");
        return;
    }
    life -= 1;
    logBoite(name + " court et perd 1 point de vie !");
    updateStatus();
}

function seBattre()
{
    if(!enVie)
    {
        logBoite(name + " n'est plus... Il ne peut pas se battre.");
        return;
    }
    if(!awake)
    {
        logBoite(name + " fait sa sièste et ne répond pas.");
        return;
    }
    if(life <= 3)
    {
        logBoite(name + " n'a pas assez de vie pour se battre !");
        return;
    }
    life -= 3;
    logBoite(name + " se bat -> -3 vie.(Ne faites pas ça chez vous)");
    updateStatus();
}

function travailler()
{
    if(!enVie)
    {
        logBoite(name + " n'est plus... Il ne peut pas travailler.");
        return;
    }
    if(!awake)
    {
        logBoite(name + " fait sa sièste et ne répond pas.");
        return;
    }
    if(life <= 1)
    {
        logBoite(name + " n'a pas assez de vie pour travailler !");
        return;
    }
    life -= 1;
    money += 2;
    logBoite(name + " travaille -> +2 money & -1 vie.");
    updateStatus();
}

function manger()
{
    if(!enVie)
    {
        logBoite(name + " n'est plus... Il ne peut pas manger.");
        return;
    }
    if(!awake)
    {
        logBoite(name + " fait sa sièste et ne répond pas.");
        return;
    }
    if(money <= 3)
    {
        logBoite(name + " est pauvre.");
        return;
    }
    money -= 3;
    life += 2;
    logBoite(name + " mange -> +2 vie & -3 money.");
    updateStatus();
}

function dormir()
{
    if(!enVie)
    {
        logBoite(name + " n'est plus... Il ne peut pas dormir.");
        return;
    }
    if(!awake)
    {
        logBoite(name + " dort déjà.");
        return;
    }
    eatButton.disabled = true;
    workButton.disabled = true;
    fightButton.disabled = true;
    runButton.disabled = true;
    sleepButton.disabled = true;    
    lotoButton.disabled = true;
    logBoite(name + " s'endort...");
    awake = false;
    updateStatus();
    setTimeout(() => {
        if(enVie)
        {
            awake = true;
            life += 1;
            runButton.disabled = false;
            fightButton.disabled = false;
            workButton.disabled = false;
            eatButton.disabled = false;
            sleepButton.disabled = false;
            lotoButton.disabled = false;
            updateStatus();
            logBoite(name + " se réveille !");
        }
    }, 5000);
}

function actionauhasard()
{
    const actions = [courir, seBattre, travailler, manger, dormir];
    const index = Math.floor(Math.random() * actions.length);
    actions[index]();
}

function tuerMonstre()
{
    if(enVie)
    {
        enVie = false;
        logBoite(name + " nous a quitté...");
        life = 0;
        money = 0;
        awake = false;
        killButton.disabled = true;
        runButton.disabled = true;
        fightButton.disabled = true;
        workButton.disabled = true;
        eatButton.disabled = true;
        sleepButton.disabled = true;
        lotoButton.disabled = true;
        updateStatus();
    }
    else
    {
        logBoite(name + " est déjà mort !");
    }
}

function ressusciterMonstre()
{
    if(!enVie)
    {
        enVie = true;
        awake = true;
        go();
        logBoite(name + " est de retour parmis nous !");
        killButton.disabled = false;
        runButton.disabled = false;
        fightButton.disabled = false;
        workButton.disabled = false;
        eatButton.disabled = false;
        sleepButton.disabled = false;
        lotoButton.disabled = false;
        updateStatus();
    }
    else
    {
        logBoite(name + " est en bonne santé !");
    }
}

runButton.addEventListener("click", courir);
fightButton.addEventListener("click", seBattre);
workButton.addEventListener("click", travailler);
eatButton.addEventListener("click", manger);
sleepButton.addEventListener("click", dormir);

killButton.addEventListener("click", tuerMonstre);
newButton.addEventListener("click", ressusciterMonstre);

var monster = document.getElementById("monster");

function updateMonsterColor() {
    if (life < 10)
        monster.style.backgroundColor = "red";
    else if(life < 20)
        monster.style.backgroundColor = "green";
    else
        monster.style.backgroundColor = "blue";
}

const changeNameButton = document.getElementById("changeNameButton");

function renommer()
{
    const newName = prompt("Entrez le nouveau nom du monstre :");
    if(newName)
    {
        name = newName;
        updateStatus();
    }
}

changeNameButton.addEventListener("click", renommer);

function playLoto() {
    const chanceDeGagner = 0.1;
    const aGagne = Math.random() < chanceDeGagner;
    if(life > 1 && money > 1)
    {
        life--;
        money--;
        if (aGagne) {
          money += 500;
          logBoite("Félicitations " + name + " a gagné le jackpot !!");
          updateStatus();
          return true;
        } else {
          logBoite(name + " a perdu au loto...");
          updateStatus();
          return false;
        }
    }
    else
        logBoite(name + " n'est pas assez d'argent ou de vie pour jouer au loto.");
}

lotoButton.addEventListener("click", playLoto);

function Victoire()
{
    if(money > 500)
    {
        logBoite("Fin du jeu le monstre a gagné !");
        runButton.disabled = true;
        fightButton.disabled = true;
        workButton.disabled = true;
        eatButton.disabled = true;
        sleepButton.disabled = true;
        lotoButton.disabled = true;
        alert("Fin du jeu le monstre a gagné !");
        location.reload();
        return;
    }
}

function updatehp() {
    const hp = document.getElementById("hp");
    const pourcentage = (life / maxLife) * 100;
    hp.style.width = pourcentage + '%';

    if (pourcentage > 80)
        hp.style.backgroundColor = "blue";
    else if (pourcentage > 60)
        hp.style.backgroundColor = "green";
    else if(pourcentage > 30)
        hp.style.backgroundColor = "orange";
    else
        hp.style.backgroundColor = "red";
}