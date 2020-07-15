/*-----
partially reverse-engineered https://david.blob.core.windows.net/softengine3d/part3/index.html
added compatibility with newer Babylon export formats

try this in your browser:
copy(BABYLON.SceneSerializer.SerializeMesh(BABYLON.MeshBuilder.CreateBox("bla", {}, scene), true, true))

What's possible with the MeshBuilder? See for example https://doc.babylonjs.com/how_to/polyhedra_shapes
------*/

var fs = require('fs');

var args = process.argv.slice(2);
var jsonObject = JSON.parse(fs.readFileSync(args[0], 'utf8'));

var result =
    "#include <libgba-bitmap-engine/mesh.h>\n" +
    "Mesh* createMesh() { \n" +
    "\t auto obj = new Mesh();\n";

var vertices = 0;
function addMesh(x, y, z) {
    result += `\t obj->add(VectorFx::fromFloat(${x}, ${y}, ${z}));\n`;
    vertices++;
}
function addMeshWithNormals(x, y, z, nx, ny, nz) {
    result += `\t obj->add(VectorFx::fromFloat(${x}, ${y}, ${z}), VectorFx::fromFloat(${nx}, ${ny}, ${nz}));\n`;
    vertices++;
}
var faces = 0;
function addFace(a, b, c) {
    result += `\t obj->addFace(${a}, ${b}, ${c});\n`;
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
    const mesh = jsonObject.meshes[meshIndex];
    var data = undefined;
    var verticesArray = mesh.vertices;
    var indicesArray = mesh.indices;
    var uvCount = mesh.uvCount;
    if(mesh.geometryId) {
        data = jsonObject.geometries.vertexData.find(v => v.id === mesh.geometryId);
        verticesArray = data.positions;
        indicesArray = data.indices;
    }

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
        default:
            verticesStep = 3;
            break;
    }
    var verticesCount = verticesArray.length / verticesStep;
    var facesCount = indicesArray.length / 3;
    //var mesh = new SoftEngine.Mesh(jsonObject.meshes[meshIndex].name, verticesCount, facesCount);
    for(var index = 0; index < verticesCount; index++) {
        var x = verticesArray[index * verticesStep];
        var y = verticesArray[index * verticesStep + 1];
        var z = verticesArray[index * verticesStep + 2];

        if(x !== undefined && y !== undefined && z !== undefined) {
            if(data && data.normals) {
                var nx = data.normals[index * verticesStep];
                var ny = data.normals[index * verticesStep + 1];
                var nz = data.normals[index * verticesStep + 2];

                addMeshWithNormals(x, y, z, nx, ny, nz);
            } else {
                var nx = verticesArray[index * verticesStep + 3];
                var ny = verticesArray[index * verticesStep + 4];
                var nz = verticesArray[index * verticesStep + 5];

                addMeshWithNormals(x, y, z, nx, ny, nz);
            }
        } else {
            console.log(`WARN; vertices index ${index} with step ${verticesStep} contains invalid data: ${x}, ${y}, ${z}`)
        }
    }
    for(var index = 0; index < facesCount; index++) {
        var a = indicesArray[index * 3];
        var b = indicesArray[index * 3 + 1];
        var c = indicesArray[index * 3 + 2];

        if(a !== undefined && b !== undefined && c !== undefined) {
            addFace(a, b, c);
        } else {
            console.log(`WARN; indices index ${index} contains invalid data: ${a}, ${b}, ${c}`)
        }
    }
    var position = mesh.position;
    setPosition(position);
}

done();
fs.writeFileSync('src/mesh.cpp', result);
console.log(`mesh.cpp written; ${vertices} vertices and ${faces} faces. GLHF!`)
if(vertices > 800) {
    console.log('WARNING lots of vertices detected, this will not run well...');
}