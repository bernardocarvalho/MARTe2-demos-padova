/**
 * Default plugin renderer for FixedGAMExample1.
 */
class FixedGAMExample1 extends MARTeObject {

    /**
     * NOOP
     */
    constructor() {
        super();
    }


    /**
     * Creates the canvas.
     *
     * @param {obj} target the target HTML container where to display the data.
     */
    prepareDisplay(target) {
        this.targetDivText = document.createElement("div");
        this.targetDivText.setAttribute("class", "divPendulumText");
        target.appendChild(this.targetDivText);
        this.refresh(100);
    }

    /**
     * Updates the pendulum with the new angle position.
     *
     * @param {obj} jsonData the data as received by the server and which should contain the angle.
     */
    displayData(jsonData) {
        //console.log(jsonData);
        this.targetDivText.innerHTML = JSON.stringify(jsonData["InputSignals"], null, '\t');
    }

    /**
     * Example of a method that can be called after instantiation.
     * @param {color} c the new widget color.
     */
     setColor(c) {
         this.targetDivText.style.color = c;
     }
}

