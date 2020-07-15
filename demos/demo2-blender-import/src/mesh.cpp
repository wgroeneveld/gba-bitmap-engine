#include <libgba-bitmap-engine/mesh.h>
Mesh* createMesh() { 
	 auto obj = new Mesh();
	 obj->add(VectorFx::fromFloat(0, 0, 1.414214), VectorFx::fromFloat(0.5773502691896258, 0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(1.414214, 0, 0), VectorFx::fromFloat(0.5773502691896258, 0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, 1.414214, 0), VectorFx::fromFloat(0.5773502691896258, 0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, 0, 1.414214), VectorFx::fromFloat(-0.5773502691896258, 0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, 1.414214, 0), VectorFx::fromFloat(-0.5773502691896258, 0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(-1.414214, 0, 0), VectorFx::fromFloat(-0.5773502691896258, 0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, 0, 1.414214), VectorFx::fromFloat(-0.5773502691896258, -0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(-1.414214, 0, 0), VectorFx::fromFloat(-0.5773502691896258, -0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, -1.414214, 0), VectorFx::fromFloat(-0.5773502691896258, -0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, 0, 1.414214), VectorFx::fromFloat(0.5773502691896258, -0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, -1.414214, 0), VectorFx::fromFloat(0.5773502691896258, -0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(1.414214, 0, 0), VectorFx::fromFloat(0.5773502691896258, -0.5773502691896258, 0.5773502691896258));
	 obj->add(VectorFx::fromFloat(1.414214, 0, 0), VectorFx::fromFloat(0.5773502691896258, -0.5773502691896258, -0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, -1.414214, 0), VectorFx::fromFloat(0.5773502691896258, -0.5773502691896258, -0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, 0, -1.414214), VectorFx::fromFloat(0.5773502691896258, -0.5773502691896258, -0.5773502691896258));
	 obj->add(VectorFx::fromFloat(1.414214, 0, 0), VectorFx::fromFloat(0.5773502691896258, 0.5773502691896258, -0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, 0, -1.414214), VectorFx::fromFloat(0.5773502691896258, 0.5773502691896258, -0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, 1.414214, 0), VectorFx::fromFloat(0.5773502691896258, 0.5773502691896258, -0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, 1.414214, 0), VectorFx::fromFloat(-0.5773502691896258, 0.5773502691896258, -0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, 0, -1.414214), VectorFx::fromFloat(-0.5773502691896258, 0.5773502691896258, -0.5773502691896258));
	 obj->add(VectorFx::fromFloat(-1.414214, 0, 0), VectorFx::fromFloat(-0.5773502691896258, 0.5773502691896258, -0.5773502691896258));
	 obj->add(VectorFx::fromFloat(-1.414214, 0, 0), VectorFx::fromFloat(-0.5773502691896258, -0.5773502691896258, -0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, 0, -1.414214), VectorFx::fromFloat(-0.5773502691896258, -0.5773502691896258, -0.5773502691896258));
	 obj->add(VectorFx::fromFloat(0, -1.414214, 0), VectorFx::fromFloat(-0.5773502691896258, -0.5773502691896258, -0.5773502691896258));
	 obj->addFace(0, 2, 1);
	 obj->addFace(3, 5, 4);
	 obj->addFace(6, 8, 7);
	 obj->addFace(9, 11, 10);
	 obj->addFace(12, 14, 13);
	 obj->addFace(15, 17, 16);
	 obj->addFace(18, 20, 19);
	 obj->addFace(21, 23, 22);
	 obj->setPosition(VectorFx::fromInt(0, 0, 0));
	 return obj;
}
