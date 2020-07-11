var fs = require('fs');

var jsonObject = JSON.parse(fs.readFileSync('monkey.babylon', 'utf8'));

var result =
    "#include <libgba-sprite-engine/mesh.h>\n" +
    "Mesh* createMesh() { \n" +
    "\t auto obj = new Mesh();\n";

var meshes = 0;
function addMesh(x, y, z) {
    result += `\t obj->add(VectorFx::fromInt(${x}, ${y}, ${z}));\n`;
    meshes++;
}
var faces = 0;
function addFace(a, b, c) {
    result += `\t obj->addFace({ ${a}, ${b}, ${c}});\n`;
    faces++;
}
function setPosition(position) {
    result += `\t obj->setPosition(VectorFx::fromInt(${position[0]}, ${position[1]}, ${position[2]}));\n`;
}
function done() {
    result +=
        "\t return obj;\n" +
        "}\n"
}

for(var meshIndex = 0; meshIndex < jsonObject.meshes.length; meshIndex++) {
    var verticesArray = jsonObject.meshes[meshIndex].vertices;
    var indicesArray = jsonObject.meshes[meshIndex].indices;
    var uvCount = jsonObject.meshes[meshIndex].uvCount;
    var verticesStep = 1;
    switch(uvCount) {
        case 0:
            verticesStep = 6;
            break;
        case 1:
            verticesStep = 8;
            break;
        case 2:
            verticesStep = 10;
            break;
    }
    var verticesCount = verticesArray.length / verticesStep;
    var facesCount = indicesArray.length / 3;
    //var mesh = new SoftEngine.Mesh(jsonObject.meshes[meshIndex].name, verticesCount, facesCount);
    for(var index = 0; index < verticesCount; index++) {
        var x = verticesArray[index * verticesStep];
        var y = verticesArray[index * verticesStep + 1];
        var z = verticesArray[index * verticesStep + 2];

        addMesh(x, y, z);
    }
    for(var index = 0; index < facesCount; index++) {
        var a = indicesArray[index * 3];
        var b = indicesArray[index * 3 + 1];
        var c = indicesArray[index * 3 + 2];

        addFace(a, b, c);
    }
    var position = jsonObject.meshes[meshIndex].position;
    setPosition(position);
}

done();
fs.writeFileSync('src/mesh.cpp', result);
console.log(`mesh.cpp written; ${meshes} meshes and ${faces} faces. GLHF!`)
