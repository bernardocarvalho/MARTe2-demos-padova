class ChartExample1 extends MARTeObject {

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
        this.canvas = document.createElement("canvas");
        this.canvas.setAttribute("class", "pendulumCanvas");
        this.canvas.width = 500;
        this.canvas.height = 600;
        var ctx = this.canvas.getContext("2d");
        target.innerHTML = "";
        target.appendChild(this.canvas);

        var xlabel = ""; 
        var ylabel = "";
        var xtype = "linear";
        var ytype = "linear";
        this.appendData  = true;
        var chart = new Chart(ctx, {
            // The type of chart we want to create
            type: "line",
            // The data for our dataset
            data: {
                datasets: [
                    {
                        label: "",
                        fill: "false",
                        backgroundColor: "white",
                        borderColor: "blue" // The main line color
                    }
                ]
            },
            // Configuration options go here
            options: {
                scales: {
                    xAxes: [{
                        type: xtype,
                        position: "bottom",
                        scaleLabel: {
                            display: true,
                            labelString: xlabel
                        }
                    }],
                    yAxes: [{
                        type: ytype,
                        scaleLabel: {
                            display: true,
                            labelString: ylabel
                        }
                    }]
                },
                responsive : false
            }
        });
        this.maxPoints = 100;
        chart.update();
        this.chart = chart;
        this.refresh(100);
    }

    /**
     * Updates the pendulum with the new angle position.
     *
     * @param {obj} jsonData the data as received by the server and which should contain the angle.
     */
    displayData(jsonData) {
        //console.log(jsonData);
        //this.targetDivText.innerHTML = JSON.stringify(jsonData["InputSignals"], null, '\t');
        if (this.xx !== undefined && this.yy !== undefined) {
            var xval = jsonData["InputSignals"][this.xx];
            var yval = jsonData["InputSignals"][this.yy];
            var data = this.chart.data.datasets[0].data;
            if (data.length > this.maxPoints) {
                data.shift();
            }
            this.chart.data.datasets[0].data.push({x: xval, y: yval});
            this.chart.update();
        }
    }

    reset() {
        this.chart.data.datasets[0].data = [];
        for (var i=0; i<this.chart.data.datasets.length; i++) {
            this.chart.data.datasets[i].data = [];
        }
    }

     setXSignal(xx) {
         this.xx = xx;
     }
          
     setYSignal(yy) {
         this.yy = yy;
     }

     setMaxPoints() {
         this.maxPoints = maxPoints;
     }
}

