import MVVM from "./mvvm/mvvm";
import { menus } from "./menu";

class Main {

    private mainMenu : HTMLDivElement;

    constructor(parameters) {

        this.mainMenu = document.getElementById("main-menu") as HTMLDivElement;

    }
}

var main = new Main(null);

