/** MARICIC Alexandre 11803731
    CHEN Kévin 11807217 **/
    
var name = "";
var life = 42;
var money = 404;
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

initMonstre("Michel", 42, 404);
afficheMonstre();
console.log("Initialisation terminer !")

function go()
{
    initMonstre("Michel", 42, 404);
    showButton.addEventListener("click", afficheMonstre);
    updateStatus();
    updateMonsterColor();
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
    let message = "Nom: " + name + ", Vie: " + life + ", Argent: " + money + ", Réveillé: " + awake;
    statut.innerHTML = message;
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
    updateMonsterColor();
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
    updateMonsterColor();
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
    updateMonsterColor();
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
    updateMonsterColor();
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
    updateMonsterColor();
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
    logBoite(name + " s'endort...");
    awake = false;
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
            updateStatus();
            logBoite(name + " se réveille !");
        }
    }, 5000);
    updateMonsterColor();
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
        updateStatus();
    }
    else
        logBoite(name + " est déjà mort !");
}

function ressusciterMonstre()
{
    if(!enVie)
    {
        enVie = true;
        life = 42;
        money = 404;
        awake = true;
        logBoite(name + " est de retour parmis nous !");
        killButton.disabled = false;
        runButton.disabled = false;
        fightButton.disabled = false;
        workButton.disabled = false;
        eatButton.disabled = false;
        sleepButton.disabled = false;
        updateStatus();
    }
    else
        logBoite(name + " est en bonne santé !");
}

var monster = document.getElementById("monster");

function updateMonsterColor()
{
    if (life < 10)
        monster.style.backgroundColor = "red";
    else if(life < 20)
        monster.style.backgroundColor = "green";
    else
        monster.style.backgroundColor = "blue";
}

runButton.addEventListener("click", courir);
fightButton.addEventListener("click", seBattre);
workButton.addEventListener("click", travailler);
eatButton.addEventListener("click", manger);
sleepButton.addEventListener("click", dormir);

killButton.addEventListener("click", tuerMonstre);
newButton.addEventListener("click", ressusciterMonstre);