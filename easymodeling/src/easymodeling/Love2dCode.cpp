
#include "Love2dCode.h"
#include "Context.h"
#include "StagePanel.h"

#include <easymodeling.h>
#include <easyshape.h>

using namespace emodeling;
using namespace ebuilder;

Love2dCode::Love2dCode(CodeGenerator& gen)
	: m_gen(gen)
{
	m_genHeader = new CodeGenerator;
	m_genLoad = new CodeGenerator;
	m_genUpdate = new CodeGenerator;
	m_genDraw = new CodeGenerator;

	m_gen.block(*m_genHeader);
	m_gen.line();
	m_gen.block(*m_genLoad);
	m_gen.line();
	m_gen.block(*m_genUpdate);
	m_gen.line();
	m_gen.block(*m_genDraw);
}

void Love2dCode::resolve()
{
	resolveHeader();
	resolveLoad();
	resolveUpdate();
	resolveDraw();
}

void Love2dCode::resolveHeader()
{
	CodeGenerator& gen = *m_genHeader;

	// actors = {}
	lua::assign(gen, "", "actors", "{}");
}

void Love2dCode::resolveLoad()
{
	CodeGenerator& gen = *m_genLoad;

	// function love.load()
	lua::Function func(gen, "love.load", 0);

	resolveLoadImages();
	gen.line();
	resolveLoadWorld();
	gen.line();
	resolveLoadBodies();
	gen.line();
	resolveLoadJoints();
}

void Love2dCode::resolveLoadImages()
{
	CodeGenerator& gen = *m_genLoad;

	// -- Load images.
	gen.line("-- Load images.");

	gen.line("images = {");
	gen.tab();

	std::vector<libmodeling::Body*> bodies;
	Context::Instance()->stage->traverseBodies(d2d::FetchAllVisitor<libmodeling::Body>(bodies));
	std::map<std::string, std::string> mapNamePath;
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		libmodeling::Body* body = bodies[i];
		std::string path = body->sprite->GetSymbol().GetFilepath();
		std::string name = d2d::FilenameTools::getFilename(path);
		mapNamePath.insert(std::make_pair(name, path));
	}

	std::map<std::string, std::string>::iterator itr = mapNamePath.begin();
	for ( ; itr != mapNamePath.end(); ++itr)
	{
		// "path"
		std::string path = "\""+d2d::FilenameTools::getFilenameWithExtension(itr->second)+"\"";
		// love.graphics.newImage("path"),
		std::string str = lua::call("", "love.graphics.newImage", 1, path.c_str()) + ",";
		// name = love.graphics.newImage("path"),
		lua::assign(gen, "", itr->first, str);
	}

	gen.detab();
	gen.line("}");
}

void Love2dCode::resolveLoadWorld()
{
	CodeGenerator& gen = *m_genLoad;

	const libmodeling::World* world = Context::Instance()->world;

	// -- Create the world.
	gen.line("-- Create the world.");
	// world = love.physics.newWorld(x, y)
	std::string x = wxString::FromDouble(world->gravity.x, 1);
	std::string y = wxString::FromDouble(-world->gravity.y, 1); // love2d's origin is left-top
	lua::assign(gen, "", "world", lua::call("", "love.physics.newWorld", 2, x.c_str(), y.c_str()));
	// world:setAllowSleeping()
	lua::call("world", "setAllowSleeping", 1, world->allowSleep ? "true" : "false");
}

void Love2dCode::resolveLoadBodies()
{
	CodeGenerator& gen = *m_genLoad;

	// -- Create actors.
	gen.line("-- Create actors.");
	// local shape
	gen.line("local shape");
	// local fixtures
	gen.line("local fixtures");
	std::vector<libmodeling::Body*> bodies;
	Context::Instance()->stage->traverseBodies(d2d::FetchAllVisitor<libmodeling::Body>(bodies));
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		libmodeling::Body* body = bodies[i];

		gen.line();

		std::string name = d2d::FilenameTools::getFilename(body->sprite->GetSymbol().GetFilepath());

		// local actor = {}
		lua::assign(gen, "local", name, "{}");

		// actor.body = love.physics.newBody(world, x, y, "dynamic")
		std::string x = wxString::FromDouble(body->sprite->GetPosition().x);
		std::string y = wxString::FromDouble(-body->sprite->GetPosition().y);
		std::string type;
		switch (body->type)
		{
		case libmodeling::Body::e_static:
			type = "\"static\"";                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
			break;
		case libmodeling::Body::e_dynamic:
			type = "\"dynamic\"";
			break;
		case libmodeling::Body::e_kinematic:
			type = "\"kinematic\"";
			break;
		}
		std::string newBody = lua::call("", "love.physics.newBody", 4, "world", x.c_str(), y.c_str(), type.c_str());
		lua::assign(gen, "", name+".body", newBody);

		if (body->sprite->GetAngle() != 0)
		{
			// actor.body:setAngle(angle)
			std::string angle = wxString::FromDouble(body->sprite->GetAngle(), 2);
			lua::call(gen, name+".body", "setAngle", 1, angle.c_str());
		}

		// actor.image = image
		std::string newImage = "images." + name;
		lua::assign(gen, "", name+".image", newImage);

		if (body->linearDamping != 0)
		{
			// actor.body:setLinearDamping(damping)
			std::string damping = wxString::FromDouble(body->linearDamping, 1);
			lua::call(gen, name+".body", "setLinearDamping", 1, damping.c_str());
		}

		if (body->angularDamping != 0)
		{
			// actor.body:setAngularDamping(damping)
			std::string damping = wxString::FromDouble(body->angularDamping, 1);
			lua::call(gen, name+".body", "setAngularDamping", 1, damping.c_str());
		}

		if (!body->allowSleep)
		{
			// actor.body:setSleepingAllowed(false)
			lua::call(gen, name+".body", "setSleepingAllowed", 1, "false");
		}

		if (body->bullet)
		{
			// actor.body:setBullet(true)
			lua::call(gen, name+".body", "setBullet", 1, "true");
		}

		if (!body->active)
		{
			// actor.body:setActive(false)
			lua::call(gen, name+".body", "setActive", 1, "false");
		}

		if (body->gravityScale != 1)
		{
			// actor.body:setGravityScale(scale)
			std::string scale = wxString::FromDouble(body->gravityScale);
			lua::call(gen, name+".body", "setGravityScale", 1, scale.c_str());
		}

		// resolve fixtures
		resolveLoadFixtures(body);

		// table.insert(actors, actor)
		lua::call(gen, "", "table.insert", 2, "actors", name.c_str());
	}
}

void Love2dCode::resolveLoadFixtures(libmodeling::Body* body)
{
	CodeGenerator& gen = *m_genLoad;

	for (size_t i = 0, n = body->fixtures.size(); i < n; ++i)
	{
		libmodeling::Fixture* fData = body->fixtures[i];

		std::string newShape;
		if (libshape::CircleShape* circle = dynamic_cast<libshape::CircleShape*>(fData->shape))
		{
			std::string radius = wxString::FromDouble(circle->radius, 1);
			if (circle->center.x == 0 && circle->center.y == 0)
			{
				// love.physics.newCircleShape(radius)
				newShape = lua::call("", "love.physics.newCircleShape", 1, radius.c_str());
			}
			else
			{
				// love.physics.newCircleShape(x, y, radius)
				std::string x = wxString::FromDouble(circle->center.x, 1);
				std::string y = wxString::FromDouble(-circle->center.y, 1);
				newShape = lua::call("", "love.physics.newCircleShape", 3, x.c_str(), y.c_str(), radius.c_str());
			}
		}
		else if (libshape::RectShape* rect = dynamic_cast<libshape::RectShape*>(fData->shape))
		{
			// love.physics.newRectangleShape(x, y, w, h)
			std::string x = wxString::FromDouble((rect->m_rect.xMax + rect->m_rect.xMin) * 0.5f, 1),
				y = wxString::FromDouble(-(rect->m_rect.yMax + rect->m_rect.yMin) * 0.5f, 1);
			std::string w = wxString::FromDouble(rect->m_rect.xMax - rect->m_rect.xMin, 1),
				h = wxString::FromDouble(rect->m_rect.yMax - rect->m_rect.yMin, 1);
			newShape = lua::call("", "love.physics.newRectangleShape", 4, x.c_str(), y.c_str(), w.c_str(), h.c_str());
		}
		else if (libshape::PolygonShape* polygon = dynamic_cast<libshape::PolygonShape*>(fData->shape))
		{
			// love.physics.newPolygonShape(size, x0, y0, x1, y1, x2, y2, ... , xn, yn)
			const std::vector<d2d::Vector>& vertices = polygon->GetVertices();
			std::string strParams;
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
			{
				std::string x = wxString::FromDouble(vertices[i].x);
				std::string y = wxString::FromDouble(-vertices[i].y);
				if (i == 0)
					strParams += x+", "+y;
				else
					strParams += ", "+x+", "+y;
			}
			newShape = lua::call("", "love.physics.newPolygonShape", 1, strParams.c_str());
		}
		else if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(fData->shape))
		{
			// love.physics.newChainShape(loop, size, x0, y0, x1, y1, x2, y2, ... , xn, yn)
			std::string strParams = chain->IsClosed() ? "true" : "false";
			const std::vector<d2d::Vector>& vertices = chain->GetVertices();
			std::string size = wxString::FromDouble(vertices.size() * 2, 1);
			strParams += ", "+size;
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
			{
				std::string x = wxString::FromDouble(vertices[i].x);
				std::string y = wxString::FromDouble(-vertices[i].y);
				strParams += ", "+x+", "+y;
			}
			newShape = lua::call("", "love.physics.newChainShape", 1, strParams.c_str());
		}


		// local shape = newShape
		lua::assign(gen, "", "shape", newShape);
		// local fixture = love.physics.newFixture(body, shape)
		std::string sBody = d2d::FilenameTools::getFilename(body->sprite->GetSymbol().GetFilepath())+".body";
		lua::assign(gen, "", "fixture", lua::call("", "love.physics.newFixture", 2, sBody.c_str(), "shape"));

		if (fData->density != 1)
		{
			// fixture:setDensity(density)
			std::string density = wxString::FromDouble(fData->density, 1);
			lua::call(gen, "fixture", "setDensity", 1, density.c_str());
		}
		if (fData->friction != 0.2f)
		{
			// fixture:setFriction(friction)
			std::string friction = wxString::FromDouble(fData->friction, 1);
			lua::call(gen, "fixture", "setFriction", 1, friction.c_str());
		}
		if (fData->restitution != 0)
		{
			// fixture:setRestitution(restitution)
			std::string restitution = wxString::FromDouble(fData->restitution, 1);
			lua::call(gen, "fixture", "setRestitution", 1, restitution.c_str());
		}
		if (fData->isSensor)
		{
			// fixture:setSensor(true)
			lua::call(gen, "fixture", "setSensor", 1, "true");
		}
		if (fData->categoryBits != 0x0001 || fData->maskBits != 0xFFFF || fData->groupIndex != 0)
		{
			// fixture:setFilterData(categoryBits, maskBits, groupIndex)
			std::string categoryBits = wxString::FromDouble(fData->categoryBits);
			std::string maskBits = wxString::FromDouble(fData->maskBits);
			std::string groupIndex = wxString::FromDouble(fData->groupIndex);
			lua::call(gen, "fixture", "setFilterData", 3, categoryBits.c_str(), maskBits.c_str(), groupIndex.c_str());
		}
	}
}

void Love2dCode::resolveLoadJoints()
{
	CodeGenerator& gen = *m_genLoad;

	// -- Create joints.
	gen.line("-- Create joints.");

	std::vector<libmodeling::Joint*> joints;
	Context::Instance()->stage->traverseJoints(d2d::FetchAllVisitor<libmodeling::Joint>(joints));
	// move gear joint to the end
	size_t iLast = joints.size() - 1;
	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		if (joints[i]->type == libmodeling::Joint::e_gearJoint)
		{
			libmodeling::Joint* tmp = joints[i];
			joints[i] = joints[iLast];
			joints[iLast] = tmp;
			--iLast;
		}
	}

	std::vector<libmodeling::Body*> bodies;
	Context::Instance()->stage->traverseBodies(d2d::FetchAllVisitor<libmodeling::Body>(bodies));
	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		gen.line();
		resolveLoadJoint(joints, i, bodies);
	}
}

void Love2dCode::resolveLoadJoint(const std::vector<libmodeling::Joint*>& joints, size_t index,
								  const std::vector<libmodeling::Body*>& bodies)
{
	libmodeling::Joint* j = joints[index];

	CodeGenerator& gen = *m_genLoad;

	std::string name = "joint" + wxString::FromDouble(index);

	size_t iA = 0, iB = 0;
	for (size_t n = bodies.size(); iA < n; ++iA)
		if (bodies[iA] == j->bodyA) break;
	for (size_t n = bodies.size(); iB < n; ++iB)
		if (bodies[iB] == j->bodyB) break;

	std::string body1 = wxString::FromDouble(iA + 1);
	body1 = "actors["+body1+"].body";

	std::string body2 = wxString::FromDouble(iB + 1);
	body2 = "actors["+body2+"].body";

	std::string collideConnected = j->collideConnected ? "true" : "false";

	switch (j->type)
	{
	case libmodeling::Joint::e_revoluteJoint:
		{
			libmodeling::RevoluteJoint* joint = static_cast<libmodeling::RevoluteJoint*>(j);

			d2d::Vector anchorA = joint->getWorldAnchorA();
			std::string x = wxString::FromDouble(anchorA.x, 1),
				y = wxString::FromDouble(-anchorA.y, 1);

			// local joint = love.physics.newRevoluteJoint(body1, body2, x, y, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newRevoluteJoint", 5, 
				body1.c_str(), body2.c_str(), x.c_str(), y.c_str(), collideConnected.c_str());
			lua::assign(gen, "local", name, newJoint);

			if (joint->enableLimit)
			{
				// joint:enableLimit(true)
				lua::call(gen, name, "enableLimit", 1, "true");
			}
			if (joint->lowerAngle != 0 || joint->upperAngle != 0)
			{
				// joint:setLimits(lowerAngle, upperAngle)
				std::string lowerAngle = wxString::FromDouble(joint->lowerAngle, 2),
					upperAngle = wxString::FromDouble(joint->upperAngle, 2);
				lua::call(gen, name, "setLimits", 2, lowerAngle.c_str(), upperAngle.c_str());
			}
			if (joint->enableMotor)
			{
				// joint:enableMotor(true)
				lua::call(gen, name, "enableMotor", 1, "true");
			}
			if (joint->maxMotorTorque != 0)
			{
				// joint:setMaxMotorTorque(maxMotorTorque)
				std::string maxMotorTorque = wxString::FromDouble(joint->maxMotorTorque, 1);
				lua::call(gen, name, "setMaxMotorTorque", 1, maxMotorTorque.c_str());
			}
			if (joint->motorSpeed != 0)
			{
				// joint:setMotorSpeed(motorSpeed)
				std::string motorSpeed = wxString::FromDouble(joint->motorSpeed, 1);
				lua::call(gen, name, "setMotorSpeed", 1, motorSpeed.c_str());
			}
		}
		break;
	case libmodeling::Joint::e_prismaticJoint:
		{
			libmodeling::PrismaticJoint* joint = static_cast<libmodeling::PrismaticJoint*>(j);

			d2d::Vector anchorA = joint->getWorldAnchorA(),
				anchorB = joint->getWorldAnchorB();
			std::string xA = wxString::FromDouble(anchorA.x, 1),
				yA = wxString::FromDouble(-anchorA.y, 1);
			std::string xB = wxString::FromDouble(anchorB.x, 1),
				yB = wxString::FromDouble(-anchorB.y, 1);

			std::string ax = wxString::FromDouble(joint->localAxisA.x, 1),
				ay = wxString::FromDouble(joint->localAxisA.y, 1);

			// local joint = love.physics.newPrismaticJoint(body1, body2, xA, yA, xB, yB, ax, ay, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newPrismaticJoint", 9, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), ax.c_str(), ay.c_str(), collideConnected.c_str());
			lua::assign(gen, "local", name, newJoint);

			if (joint->enableLimit)
			{
				// joint:enableLimit(true)
				lua::call(gen, name, "enableLimit", 1, "true");
			}
			if (joint->lowerTranslation != 0 || joint->upperTranslation != 0)
			{
				// joint:setLimits(lowerAngle, upperAngle)
				std::string lowerTranslation = wxString::FromDouble(joint->lowerTranslation, 2),
					upperTranslation = wxString::FromDouble(joint->upperTranslation, 2);
				lua::call(gen, name, "setLimits", 2, lowerTranslation.c_str(), upperTranslation.c_str());
			}
			if (joint->enableMotor)
			{
				// joint:enableMotor(true)
				lua::call(gen, name, "enableMotor", 1, "true");
			}
			if (joint->maxMotorForce != 0)
			{
				// joint:setMaxMotorForce(maxMotorForce)
				std::string maxMotorForce = wxString::FromDouble(joint->maxMotorForce, 1);
				lua::call(gen, name, "setMaxMotorForce", 1, maxMotorForce.c_str());
			}
			if (joint->motorSpeed != 0)
			{
				// joint:setMotorSpeed(motorSpeed)
				std::string motorSpeed = wxString::FromDouble(joint->motorSpeed, 1);
				lua::call(gen, name, "setMotorSpeed", 1, motorSpeed.c_str());
			}
		}
		break;
	case libmodeling::Joint::e_distanceJoint:
		{
			libmodeling::DistanceJoint* joint = static_cast<libmodeling::DistanceJoint*>(j);

			d2d::Vector anchorA = joint->getWorldAnchorA(),
				anchorB = joint->getWorldAnchorB();
			std::string xA = wxString::FromDouble(anchorA.x, 1),
				yA = wxString::FromDouble(-anchorA.y, 1);
			std::string xB = wxString::FromDouble(anchorB.x, 1),
				yB = wxString::FromDouble(-anchorB.y, 1);

			// local joint = love.physics.newDistanceJoint(body1, body2, xA, yA, xB, yB, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newPrismaticJoint", 7, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), collideConnected.c_str());
			lua::assign(gen, "local", name, newJoint);

			if (joint->frequencyHz != 0)
			{
				// joint:setFrequency(frequencyHz)
				std::string frequencyHz = wxString::FromDouble(joint->frequencyHz, 1);
				lua::call(gen, name, "setFrequency", 1, frequencyHz.c_str());
			}
			if (joint->dampingRatio != 0)
			{
				// joint:setDampingRatio(dampingRatio)
				std::string dampingRatio = wxString::FromDouble(joint->dampingRatio, 1);
				lua::call(gen, name, "setDampingRatio", 1, dampingRatio.c_str());
			}
		}
		break;
	case libmodeling::Joint::e_pulleyJoint:
		{
			libmodeling::PulleyJoint* joint = static_cast<libmodeling::PulleyJoint*>(j);

			std::string gxA = wxString::FromDouble(joint->groundAnchorA.x, 1),
				gyA = wxString::FromDouble(-joint->groundAnchorA.y, 1);
			std::string gxB = wxString::FromDouble(joint->groundAnchorB.x, 1),
				gyB = wxString::FromDouble(-joint->groundAnchorB.y, 1);

			d2d::Vector anchorA = joint->getWorldAnchorA(),
				anchorB = joint->getWorldAnchorB();
			std::string xA = wxString::FromDouble(anchorA.x, 1),
				yA = wxString::FromDouble(-anchorA.y, 1);
			std::string xB = wxString::FromDouble(anchorB.x, 1),
				yB = wxString::FromDouble(-anchorB.y, 1);

			std::string ratio = wxString::FromDouble(joint->ratio, 1);

			// local joint = love.physics.newPulleyJoint(body1, body2, gxA, gyA, gxB, gyB, xA, yA, xB, yB, ratio, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newPulleyJoint", 12, 
				body1.c_str(), body2.c_str(), gxA.c_str(), gyA.c_str(), gxB.c_str(), gyB.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), ratio.c_str(), collideConnected.c_str());
			lua::assign(gen, "local", name, newJoint);
		}
		break;
	case libmodeling::Joint::e_gearJoint:
		{
			libmodeling::GearJoint* joint = static_cast<libmodeling::GearJoint*>(j);

			size_t i1 = 0, i2 = 0;
			for (size_t n = joints.size(); i1 < n; ++i1)
				if (joints[i1] == joint->joint1)
					break;
			for (size_t n = joints.size(); i2 < n; ++i2)
				if (joints[i2] == joint->joint2)
					break;

			std::string joint1 = "joint"+wxString::FromDouble(i1),
				joint2 = "joint"+wxString::FromDouble(i2);

			std::string ratio = wxString::FromDouble(joint->ratio, 1);

			// local joint = love.physics.newGearJoint(joint1, joint2, ratio, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newGearJoint", 4, 
				joint1.c_str(), joint2.c_str(), ratio.c_str(), collideConnected.c_str());
			lua::assign(gen, "local", name, newJoint);
		}
		break;
	case libmodeling::Joint::e_wheelJoint:
		{
			libmodeling::WheelJoint* joint = static_cast<libmodeling::WheelJoint*>(j);

			std::string xA = wxString::FromDouble(joint->getWorldAnchorB().x, 1),
				yA = wxString::FromDouble(-joint->getWorldAnchorB().y, 1);
			std::string ax = wxString::FromDouble(joint->localAxisA.x, 1),
				ay = wxString::FromDouble(-joint->localAxisA.y, 1);

			// local joint = love.physics.newWheelJoint(body1, body2, xA, yA, ax, ay, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newWheelJoint", 7, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), ax.c_str(), ay.c_str(), collideConnected.c_str());
			lua::assign(gen, "local", name, newJoint);

			if (joint->enableMotor)
			{
				// joint:enableMotor(true)
				lua::call(gen, name, "enableMotor", 1, "true");
			}
			if (joint->maxMotorTorque != 0)
			{
				// joint:setMaxMotorTorque(maxMotorForce)
				std::string maxMotorTorque = wxString::FromDouble(joint->maxMotorTorque, 1);
				lua::call(gen, name, "setMaxMotorTorque", 1, maxMotorTorque.c_str());
			}
			if (joint->motorSpeed != 0)
			{
				// joint:setMotorSpeed(motorSpeed)
				std::string motorSpeed = wxString::FromDouble(joint->motorSpeed, 1);
				lua::call(gen, name, "setMotorSpeed", 1, motorSpeed.c_str());
			}
			if (joint->frequencyHz != 2.0f)
			{
				// joint:setSpringFrequency(frequencyHz)
				std::string frequencyHz = wxString::FromDouble(joint->frequencyHz, 1);
				lua::call(gen, name, "setSpringFrequency", 1, frequencyHz.c_str());
			}
			if (joint->dampingRatio != 0.7f)
			{
				// joint:setSpringDampingRatio(dampingRatio)
				std::string dampingRatio = wxString::FromDouble(joint->dampingRatio, 1);
				lua::call(gen, name, "setSpringDampingRatio", 1, dampingRatio.c_str());
			}
		}
		break;
	case libmodeling::Joint::e_weldJoint:
		{
			libmodeling::WeldJoint* joint = static_cast<libmodeling::WeldJoint*>(j);

			d2d::Vector anchorA = joint->getWorldAnchorA(),
				anchorB = joint->getWorldAnchorB();
			std::string xA = wxString::FromDouble(anchorA.x, 1),
				yA = wxString::FromDouble(-anchorA.y, 1);
			std::string xB = wxString::FromDouble(anchorB.x, 1),
				yB = wxString::FromDouble(-anchorB.y, 1);

			// local joint = love.physics.newWeldJoint(body1, body2, xA, yA, xB, yB, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newWeldJoint", 7, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), collideConnected.c_str());
			lua::assign(gen, "local", name, newJoint);

			if (joint->frequencyHz != 0)
			{
				// joint:setFrequency(frequencyHz)
				std::string frequencyHz = wxString::FromDouble(joint->frequencyHz, 1);
				lua::call(gen, name, "setFrequency", 1, frequencyHz.c_str());
			}
			if (joint->dampingRatio != 0)
			{
				// joint:setDampingRatio(dampingRatio)
				std::string dampingRatio = wxString::FromDouble(joint->dampingRatio, 1);
				lua::call(gen, name, "setDampingRatio", 1, dampingRatio.c_str());
			}
		}
		break;
	case libmodeling::Joint::e_frictionJoint:
		{
			libmodeling::FrictionJoint* joint = static_cast<libmodeling::FrictionJoint*>(j);

			d2d::Vector anchorA = joint->getWorldAnchorA(),
				anchorB = joint->getWorldAnchorB();
			std::string xA = wxString::FromDouble(anchorA.x, 1),
				yA = wxString::FromDouble(-anchorA.y, 1);
			std::string xB = wxString::FromDouble(anchorB.x, 1),
				yB = wxString::FromDouble(-anchorB.y, 1);

			// local joint = love.physics.newFrictionJoint(body1, body2, xA, yA, xB, yB, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newFrictionJoint", 7, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), collideConnected.c_str());
			lua::assign(gen, "local", name, newJoint);

			if (joint->maxForce != 0)
			{
				// joint:setMaxForce(maxForce)
				std::string maxForce = wxString::FromDouble(joint->maxForce, 1);
				lua::call(gen, name, "setMaxForce", 1, maxForce.c_str());
			}
			if (joint->maxTorque != 0)
			{
				// joint:setMaxTorque(maxTorque)
				std::string maxTorque = wxString::FromDouble(joint->maxTorque, 1);
				lua::call(gen, name, "setMaxTorque", 1, maxTorque.c_str());
			}
		}
		break;
	case libmodeling::Joint::e_ropeJoint:
		{
			libmodeling::RopeJoint* joint = static_cast<libmodeling::RopeJoint*>(j);

			d2d::Vector anchorA = joint->getWorldAnchorA(),
				anchorB = joint->getWorldAnchorB();
			std::string xA = wxString::FromDouble(anchorA.x, 1),
				yA = wxString::FromDouble(-anchorA.y, 1);
			std::string xB = wxString::FromDouble(anchorB.x, 1),
				yB = wxString::FromDouble(-anchorB.y, 1);

			std::string maxLength = wxString::FromDouble(joint->maxLength, 1);

			// local joint = love.physics.newRopeJoint(body1, body2, xA, yA, xB, yB, maxLength, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newFrictionJoint", 8, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), maxLength.c_str(), collideConnected.c_str());
			lua::assign(gen, "local", name, newJoint);

		}
		break;
	case libmodeling::Joint::e_motorJoint:
		{
		}
		break;
	}
}

void Love2dCode::resolveUpdate()
{
	CodeGenerator& gen = *m_genUpdate;

	// function love.load()
	lua::Function func(gen, "love.update", 1, "dt");

	// -- Update the world.
	gen.line("-- Update the world.");
	// world:update(dt)
	lua::call(gen, "world", "update", 1, "dt");
}

void Love2dCode::resolveDraw()
{
	CodeGenerator& gen = *m_genDraw;

	// function love.draw()
	lua::Function func(gen, "love.draw", 0);

	// love.graphics.push()
	lua::call(gen, "", "love.graphics.push", 0);
	// love.graphics.translate(400, 240)
	lua::call(gen, "", "love.graphics.translate", 2, "400", "240");

	{
		// for i, v in ipairs(actors) do
		lua::For _for(gen, "i, v in ipairs(actors)");
		// love.graphics.draw(v.image, v.body:getX(), v.body:getY(), v.body:getAngle(), 1, 1, ox, oy)
		std::string ox = "v.image:getWidth() * 0.5",
			oy = "v.image:getHeight() * 0.5";
		lua::call(gen, "", "love.graphics.draw", 8, "v.image", 
			"v.body:getX()", "v.body:getY()", "v.body:getAngle()", "1", "1", ox.c_str(), oy.c_str());
	}

	// love.graphics.pop()
	lua::call(gen, "", "love.graphics.pop", 0);
}