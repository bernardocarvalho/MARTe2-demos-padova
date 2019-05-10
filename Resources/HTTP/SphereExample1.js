class SphereExample1 extends MARTeObject {

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
        this.canvas.width = "100%";
        this.canvas.height = "100%";

        this.scene = new THREE.Scene();
        this.camera = new THREE.PerspectiveCamera(75, window.innerWidth/window.innerHeight, 0.1,1000);

        this.renderer = new THREE.WebGLRenderer();
        this.renderer.setSize (window.innerWidth, window.innerHeight);
        document.body.appendChild(this.renderer.domElement);

        this.radius = 10;
        var geometry = new THREE.SphereGeometry(this.radius, 12, 12);
        var wireframe = new THREE.WireframeGeometry( geometry );
        this.sphere = new THREE.Mesh(geometry, wireframe);

        this.line = new THREE.LineSegments( wireframe );
        this.line.material.depthTest = false;
        this.line.material.transparent = false;

        this.sphere.material.transparent = false;
        this.scene.add( this.line );
        this.scene.add( this.sphere );

        this.camera.position.z = 30;
        //this.camera.position.x= 10;

        var pointGeometry = new THREE.SphereGeometry(1, 12, 12);
        var pointWireframe = new THREE.WireframeGeometry( pointGeometry);
        var pointMesh = new THREE.Mesh(pointGeometry, pointWireframe);
        this.pointRef = new THREE.LineSegments( pointWireframe );
        this.pointRef.material.depthTest = false;
        this.pointRef.material.transparent = false;
        this.scene.add( this.pointRef );

        this.pointMeas = new THREE.LineSegments( pointWireframe );
        this.pointMeas.material.depthTest = false;
        this.pointMeas.material.transparent = false;
        this.scene.add( this.pointMeas );


        this.renderer.render (this.scene, this.camera);
        this.refresh(100);
    }
    
    displayData(jsonData) {
        this.pointRef.position.x = this.radius * jsonData["InputSignals"]["Reference0"] * jsonData["InputSignals"]["Reference3"];
        this.pointRef.position.y = this.radius * jsonData["InputSignals"]["Reference0"] * jsonData["InputSignals"]["Reference2"];
        //this.pointRef.position.z = this.radius * jsonData["InputSignals"]["Reference1"];

        this.pointMeas.position.x = this.radius * jsonData["InputSignals"]["Measurement0"] * jsonData["InputSignals"]["Measurement3"];
        this.pointMeas.position.y = this.radius * jsonData["InputSignals"]["Measurement0"] * jsonData["InputSignals"]["Measurement2"];
        //this.pointMeas.position.z = this.radius * jsonData["InputSignals"]["Measurement1"];*/

        this.line.rotation.x += 0.01;
        this.line.rotation.y -= 0.01;
        this.line.rotation.z -= 0.01;
        this.sphere.rotation.x += 0.02;
        this.sphere.rotation.y += 0.02;
        this.renderer.render (this.scene, this.camera);
    }

}

