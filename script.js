/*
    $ Script
*/

// ! Changes color mode
let isDark = true;
function toggleColorMode(){
    if(isDark){
        isDark = false;
        document.getElementById("color-mode").setAttribute("src", "./imgs/light-mode.png");
        document.documentElement.style.setProperty("--color-mode", "#ffffff");
        document.documentElement.style.setProperty("--text-color", "#000000");
    } else {
        isDark = true;
        document.getElementById("color-mode").setAttribute("src", "./imgs/dark-mode.png");
        document.documentElement.style.setProperty("--color-mode", "#091723");
        document.documentElement.style.setProperty("--text-color", "#ffffff");
    }
}