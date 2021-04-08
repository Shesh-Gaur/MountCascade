#include "PhysicsBody.h"
#include "ECS.h"
#include "Astar.h"
#include "RayCastCallback.h"
bool PhysicsBody::m_drawBodies = false;
std::vector<int> PhysicsBody::m_bodiesToDelete;

PhysicsBody::PhysicsBody(int entity, b2Body * body, float radius, vec2 centerOffset, bool sensor, EntityCategories category, int collidesWith, float friction, float density)
{
	//Bodies don't reference a shape by themselves
	//they need a shape that has been linked to a fixture
	//in order to have a shape
	b2CircleShape tempShape;
	tempShape.m_p = b2Vec2(float32(centerOffset.x), float32(centerOffset.y));
	tempShape.m_radius = radius;

	//Creates the actual fixture (aka, shape, mass, etc);
	b2FixtureDef tempFixture;
	tempFixture.shape = &tempShape;
	tempFixture.density = density;
	tempFixture.friction = friction;
	tempFixture.isSensor = sensor;
	tempFixture.filter.categoryBits = category;
	tempFixture.filter.maskBits = collidesWith;
	

	m_body = body;
	m_body->CreateFixture(&tempFixture);

	m_body->SetUserData((void*)entity);

	m_body = body;
	m_bodyType = BodyType::CIRCLE;

	//Stores the position;
	m_position = m_body->GetPosition();

	m_width = radius * 2.f;
	m_height = radius * 2.f;

	m_centerOffset = centerOffset;

	
}

PhysicsBody::PhysicsBody(int entity, b2Body* body, float width, float height, vec2 centerOffset, bool sensor, EntityCategories category, int collidesWith, float friction, float density)
{
	//Bodies don't reference a shape by themselves
	//they need a shape that has been linked to a fixture
	//in order to have a shape
	b2PolygonShape tempShape;
	tempShape.SetAsBox(float32(width / 2.f), float32(height / 2.f),
		b2Vec2(float32(centerOffset.x), float32(centerOffset.y)), float32(0.f));

	//Creates the actual fixture (aka, shape, mass, etc);
	b2FixtureDef tempFixture;
	tempFixture.shape = &tempShape;
	tempFixture.density = density;
	tempFixture.friction = friction;
	tempFixture.isSensor = sensor;
	tempFixture.filter.categoryBits = category;
	tempFixture.filter.maskBits = collidesWith;

	m_body = body;
	m_body->CreateFixture(&tempFixture);

	m_body->SetUserData((void*)entity);

	m_body = body;
	m_bodyType = BodyType::BOX;

	m_width = width;
	m_height = height;

	m_centerOffset = centerOffset;
}

PhysicsBody::PhysicsBody(int entity, BodyType bodyType, b2Body* body, std::vector<b2Vec2> points, vec2 centerOffset, bool sensor, EntityCategories category, int collidesWith, float friction, float density)
{
	//Used to calculate new width and height
	float lX = 999.f;
	float rX = -999.f;
	float bY = 999.f;
	float tY = -999.f;

	//Bodies don't reference a shape by themselves
	//they need a shape that has been linked to a fixture
	//in order to have a shape
	int count = points.size();
	b2Vec2 polyPoints[8];

	if (points.size() <= 8)
	{
		for (int i = 0; i < points.size(); i++)
		{
			polyPoints[i] = points[i];

			//Used to calculate width and height values
			lX = std::min(lX, points[i].x);
			rX = std::max(rX, points[i].x);
			bY = std::min(bY, points[i].y);
			tY = std::max(tY, points[i].y);
		}
	}

	b2PolygonShape tempShape;
	tempShape.Set(polyPoints, count);
	tempShape.m_centroid = b2Vec2(centerOffset.x, centerOffset.y);

	//Creates the actual fixture (aka, shape, mass, etc);
	b2FixtureDef tempFixture;
	tempFixture.shape = &tempShape;
	tempFixture.density = density;
	tempFixture.friction = friction;
	tempFixture.isSensor = sensor;
	tempFixture.filter.categoryBits = category;
	tempFixture.filter.maskBits = collidesWith;

	m_body = body;
	m_body->CreateFixture(&tempFixture);

	m_body->SetUserData((void*)entity);

	m_body = body;
	m_bodyType = bodyType;

	m_width = rX - lX;
	m_height = tY - bY;

	m_centerOffset = centerOffset;
}

void PhysicsBody::DeleteBody()
{
	if (m_body != nullptr)
	{
		m_body->GetWorld()->DestroyBody(m_body);
	}
}

void PhysicsBody::Update(Transform * trans)
{
	if (name == "Bat")
	{

			animationFrame += 7.f * getDeltaTime();
		
			//std::cout << "\n" << animationFrame;
				std::string fileName = "bat/MR1.png";

					if (GetVelocity().x > 0) {
						switch ((int)round(animationFrame)) {
						case 0:
							fileName = "bat/MR1.png";
							break;
						case 1:
							fileName = "bat/MR2.png";
							break;
						case 2:
							fileName = "bat/MR3.png";
							break;
						case 3:
							fileName = "bat/MR4.png";
							break;
						case 4:
							fileName = "bat/MR5.png";
							break;
						case 5:
							fileName = "bat/MR6.png";
							break;
						default:
							fileName = "bat/MR1.png";
							animationFrame = 0;
							break;
						}
					}
					else if (GetVelocity().x <= 0) {
						switch ((int)round(animationFrame)) {
						case 0:
							fileName = "bat/ML1.png";
							break;
						case 1:
							fileName = "bat/ML2.png";
							break;
						case 2:
							fileName = "bat/ML3.png";
							break;
						case 3:
							fileName = "bat/ML4.png";
							break;
						case 4:
							fileName = "bat/ML5.png";
							break;
						case 5:
							fileName = "bat/ML6.png";
							break;
						default:
							fileName = "bat/ML1.png";
							animationFrame = 0;
							break;
						}
					}

					ECS::GetComponent<Sprite>((int)GetBody()->GetUserData()).LoadSprite(fileName, 16, 16);
				

			
		
		if (playerSpotted == false && getCurrentClock() % 10 == 0)
		{
			RayCastCallback viewRay;
			m_body->GetWorld()->RayCast(&viewRay, GetPosition(), ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition());

			if (viewRay.m_fixture != NULL)
			{
				if (viewRay.m_fixture->GetBody() == ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody())
				{

					playerSpotted = true;
				}
			}
		}

		if (playerSpotted == true)
		{
			SetGravityScale(0);
			GetBody()->GetFixtureList()->SetFriction(0);

			if (getCurrentClock() % 5 == 0) //Runs Pathfinding Calculations 12 times every 60 frames (5 Times more performant now!)
			{

				dispatchAI();
				SetNextMovement(CalculateMovement(getPathSet()[getPathCount() - 1].position));
				//std::cout << "\n" << sqrt(GetBody()->GetLinearVelocity().y * GetBody()->GetLinearVelocity().y + GetBody()->GetLinearVelocity().x * GetBody()->GetLinearVelocity().x);
			}


			GetBody()->ApplyLinearImpulseToCenter(b2Vec2(GetNextMovement().x * GetSpeed() * getDeltaTime(), GetNextMovement().y * GetSpeed() * getDeltaTime()), true);


			if (sqrt(GetBody()->GetLinearVelocity().y * GetBody()->GetLinearVelocity().y + GetBody()->GetLinearVelocity().x * GetBody()->GetLinearVelocity().x) > GetSpeed()/5)
			{
				GetBody()->ApplyLinearImpulseToCenter(-b2Vec2(GetBody()->GetLinearVelocity().x * (GetSpeed()/32) * getDeltaTime(), GetBody()->GetLinearVelocity().y * (GetSpeed()/32) * getDeltaTime()), true);
				
			}

				

			
			
			if (knockedBack == true)
			{
				b2Vec2 knockDir = -CalculateMovement(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition());
				GetBody()->ApplyLinearImpulseToCenter(b2Vec2(knockDir.x * 900000 * getDeltaTime(), knockDir.y * 900000 * getDeltaTime()), true);
				knockedBack = false;
			}

			
		} 
		else
		{
			SetGravityScale(-1);
			GetBody()->GetFixtureList()->SetFriction(1);

		}
	}
	else if (name == "Boss")
	{
	 //HealthBar Code
		std::string fileName2 = "Masks/SquareMask.png";
		float adj = GetHealth() / GetMaxHealth();
		ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName2, 510 * adj, 12);

		if (health <= 0)
		{
			SetPosition(b2Vec2(1000, 1000));
		}

		std::string fileName;
		ECS::GetComponent<Sprite>((int)GetBody()->GetUserData()).LoadSprite(fileName, 256, 256);

		if (GetHealth() < 6 && batSpawnCounter == 2)
		{
			spawnBatsNow = true;
			batSpawnCounter++;

		}
		else if (GetHealth() < 12 && batSpawnCounter == 1)
		{
			spawnBatsNow = true;
			batSpawnCounter++;

		}
		else if (GetHealth() < 18 && batSpawnCounter == 0)
		{
			spawnBatsNow = true;
			batSpawnCounter++;

		}


		else if (GetHealth() == 20 && batSpawnCounter != 0)
		{
			batSpawnCounter = 0;

		}

		if (playerSpotted == false && getCurrentClock() % 10 == 0)
		{
			fileName = "golem/idleframe1.png";
			ECS::GetComponent<Sprite>((int)GetBody()->GetUserData()).LoadSprite(fileName, 256, 256);

			RayCastCallback viewRay;
			m_body->GetWorld()->RayCast(&viewRay, GetPosition(), ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition());

			if (viewRay.m_fixture != NULL)
			{
				if (viewRay.m_fixture->GetBody() == ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody())
				{
					playerSpotted = true;
				}
			}
		}


		if (playerSpotted == true)
		{
			animationFrame += 5.f * getDeltaTime();
			
			if (GetPosition().y <= 138)
			{
				if (GetPosition().x > 700)
				{
					
					SetPosition(b2Vec2(-23.f, 700.f));

				}
				else
				{
					SetPosition(b2Vec2(615.f, 690.f));
					
				}
				
			}

			if (resetPosition == true)
			{
				GetBody()->SetLinearVelocity(b2Vec2(GetSpeed()* -10, GetBody()->GetLinearVelocity().y));

				resetPosition = false;
			}

			if (attackCooldown > 0)
			{
				attackCooldown -= 1.f * getDeltaTime();
			}
			//std::cout << "\nAttack Cooldown " << attackCooldown;
			if (bossLastVel == NULL || bossLastVel == 0)
			{
				bossLastVel = 10.f;

			}
			
			b2Vec2 bossDistance = GetPosition() - ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition();

			if (!isAttacking && attackCooldown <= 0)
			{
				if (sqrt(bossDistance.x * bossDistance.x + bossDistance.y * bossDistance.y) < 140.f)
				{
					animationFrame = 0.f;
					isAttacking = true;

				}

			}

			if (!isAttacking && !isCharging) //Is Walking
			{

				if (recoverCooldown > 0)
				{
					recoverCooldown -= 1.f * getDeltaTime();
				}
				else
				{
					isCharging = true;
					recoverCooldown = 0;

				}

				//std::cout << "\nRECOVER COOLDOWN " << recoverCooldown;

				if (GetVelocity().x > 60) {
					switch ((int)round(animationFrame)) {
					case 0:
						fileName = "golem/golemoppositerunframe1.png";
						break;
					case 1:
						fileName = "golem/golemoppositerunframe2.png";
						break;
					case 2:
						fileName = "golem/golemoppositerunframe3.png";
						break;
					case 3:
						fileName = "golem/golemoppositerunframe4.png";
						break;
					case 4:
						fileName = "golem/golemoppositerunframe5.png";
						break;
					default:
						fileName = "golem/golemoppositerunframe6.png";
						animationFrame = 0;
						break;
					}
					ECS::GetComponent<Sprite>((int)GetBody()->GetUserData()).LoadSprite(fileName, 256, 256);


				}
				else if (GetVelocity().x < -60) {
					switch ((int)round(animationFrame)) {
					case 0:
						fileName = "golem/runframe1.png";
						break;
					case 1:
						fileName = "golem/runframe2.png";
						break;
					case 2:
						fileName = "golem/runframe3.png";
						break;
					case 3:
						fileName = "golem/runframe4.png";
						break;
					case 4:
						fileName = "golem/runframe5.png";
						break;
					default:
						fileName = "golem/runframe6.png";
						animationFrame = 0;
						break;
					}
					ECS::GetComponent<Sprite>((int)GetBody()->GetUserData()).LoadSprite(fileName, 256, 256);

				}
				else {
					switch ((int)round(animationFrame)) {
					case 0:
						fileName = "golem/idleframe1.png";
						break;
					case 1:
						fileName = "golem/idleframe2.png";
						break;
					default:
						fileName = "golem/idleframe3.png";
						animationFrame = 0;
						break;

					}
				}
				ECS::GetComponent<Sprite>((int)GetBody()->GetUserData()).LoadSprite(fileName, 256, 256);

				




				if (getCurrentClock() % 10 == 0)
				{
					float direction2 = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - GetPosition().x;
					direction2 /= abs(direction2);
					SetNextMovement(b2Vec2(direction2 * GetSpeed(), GetBody()->GetLinearVelocity().y));
				}

				if (GetPosition().x < 225.f)
				{

					GetBody()->SetLinearVelocity(b2Vec2(GetNextMovement().x, GetNextMovement().y));
					bossLastVel = GetVelocity().x;
				}

				else if (GetNextMovement().x < 0 && GetPosition().x < 255.f)
				{
					GetBody()->SetLinearVelocity(b2Vec2(GetNextMovement().x, GetNextMovement().y));
					bossLastVel = GetVelocity().x;


				}

				//std::cout << "\nHi, I'm WALKING rn " << animationFrame;

			}
			else if (isAttacking && !isCharging)//Is Attacking
			{
				if (bossLastVel < 0) {
					switch ((int)round(animationFrame)) { //left
					case 0:
						fileName = "golem/attackanim1.png";
						break;
					case 1:
						fileName = "golem/attackanim2.png";
						break;
					case 2:
						fileName = "golem/attackanim3.png";
						break;
					case 3:
						fileName = "golem/attackanim4.png";
						break;
					case 4:
						fileName = "golem/attackanim5.png";
						break;
					default:
						fileName = "golem/attackanim6.png";
						animationFrame = 0;
						isAttacking = false;
						break;
					}
				}
				else { //right
					switch ((int)round(animationFrame)) {
					case 0:
						fileName = "golem/attackanim1r.png";
						break;
					case 1:
						fileName = "golem/attackanim2r.png";
						break;
					case 2:
						fileName = "golem/attackanim3r.png";
						break;
					case 3:
						fileName = "golem/attackanim4r.png";
						break;
					case 4:
						fileName = "golem/attackanim5r.png";
						break;
					default:
						fileName = "golem/attackanim6r.png";
						animationFrame = 0;
						isAttacking = false;
						break;

					}
				}

				ECS::GetComponent<Sprite>((int)GetBody()->GetUserData()).LoadSprite(fileName, 256, 256);
				attackCooldown = attackCooldownDefault;
				//std::cout << "\nHi, I'm attacking rn " << animationFrame;

			}

			if (isCharging && !isAttacking)//Is Charging
			{
				if (bossLastVel < 0) {
					switch ((int)round(animationFrame)) { //left
					case 0:
						fileName = "golem/golemrecharge1.png";
						break;
					case 1:
						fileName = "golem/golemrecharge2.png";
						break;

					default:
						fileName = "golem/golemrecharge3.png";
						chargeLoop--;
						animationFrame = 0;
						break;
					}
				}
				else { //right
					switch ((int)round(animationFrame)) {
					case 0:
						fileName = "golem/golemrecharge1r.png";
						break;
					case 1:
						fileName = "golem/golemrecharge2r.png";
						break;

					default:
						fileName = "golem/golemrecharge3r.png";
						chargeLoop--;
						animationFrame = 0;
						break;

					}


				}
				if (chargeLoop <= 0)
				{
					//std::cout << "\nLAST VEL " << bossLastVel;

					if (bossLastVel > 0) {
						switch ((int)round(animationFrame)) {
						case 0:
							fileName = "golem/golemoppositerunframe1.png";
							break;
						case 1:
							fileName = "golem/golemoppositerunframe2.png";
							break;
						case 2:
							fileName = "golem/golemoppositerunframe3.png";
							break;
						case 3:
							fileName = "golem/golemoppositerunframe4.png";
							break;
						case 4:
							fileName = "golem/golemoppositerunframe5.png";
							break;
						default:
							fileName = "golem/golemoppositerunframe6.png";
							animationFrame = 0;
							break;
						}
						ECS::GetComponent<Sprite>((int)GetBody()->GetUserData()).LoadSprite(fileName, 256, 256);


					}
					else if (bossLastVel < -0) {
						switch ((int)round(animationFrame)) {
						case 0:
							fileName = "golem/runframe1.png";
							break;
						case 1:
							fileName = "golem/runframe2.png";
							break;
						case 2:
							fileName = "golem/runframe3.png";
							break;
						case 3:
							fileName = "golem/runframe4.png";
							break;
						case 4:
							fileName = "golem/runframe5.png";
							break;
						default:
							fileName = "golem/runframe6.png";
							animationFrame = 0;
							break;
						}

					}

					RayCastCallback wallRay;

					if (bossLastVel != 0)
					{
						GetBody()->GetWorld()->RayCast(&wallRay, GetPosition() - b2Vec2(0.f,100.f), GetPosition() - b2Vec2(0.f, 100.f) + b2Vec2(bossLastVel * 1.5, 0));

						if (wallRay.m_fixture != NULL)
						{
							std::cout << "\n" << ECS::GetComponent<PhysicsBody>((int)wallRay.m_fixture->GetBody()->GetUserData()).GetName();
							if (ECS::GetComponent<PhysicsBody>((int)wallRay.m_fixture->GetBody()->GetUserData()).GetName() == "Wall" || ECS::GetComponent<PhysicsBody>((int)wallRay.m_fixture->GetBody()->GetUserData()).GetName() == "Decor")
							{
								recoverCooldown = recoverCooldownDefault;
								isCharging = false;
								chargeLoop = chargeLoopDefault;
							}
							else
							{
								GetBody()->SetLinearVelocity(b2Vec2(bossLastVel * 4, GetBody()->GetLinearVelocity().y));
							}
						}
						else
						{
								GetBody()->SetLinearVelocity(b2Vec2(bossLastVel * 4, GetBody()->GetLinearVelocity().y));		
						}
					}
					else
					{
						recoverCooldown = recoverCooldownDefault;
						isCharging = false;
						chargeLoop = chargeLoopDefault;

					}
					//if ray doesn't return Null, and its not the player



				}
				ECS::GetComponent<Sprite>((int)GetBody()->GetUserData()).LoadSprite(fileName, 256, 256);
				attackCooldown = attackCooldownDefault;
				//std::cout << "\nHi, I'm attacking rn " << animationFrame;

			}
		}
		SetRotationAngleDeg(0);


	}
	//Make sure that movement doesn't happen in contact step
	if (moveLater)
	{
		//set position with saved move value
		SetPosition(moveValue);
		moveLater = false;
	}
	if (rotateLater)
	{
		SetRotationAngleDeg(rotationDeg);
		rotateLater = false;
	}
	if (scaleLater)
	{
 		ScaleBody(scaleVal, scaleFixt);
		scaleLater = false;
	}

	//Stores the position;
	m_position = m_body->GetPosition();

	//Sets the transform position to the position of the physics body
	trans->SetPosition(vec3(m_body->GetPosition().x, m_body->GetPosition().y, trans->GetPosition().z));
	trans->SetRotationAngleZ(Degrees(m_body->GetAngle()));
}

void PhysicsBody::ApplyForce(vec3 force)
{
	m_body->ApplyForce(b2Vec2(float32(force.x), float32(force.y)),
						b2Vec2(float32(m_body->GetPosition().x), float32(m_body->GetPosition().y)),
						 true);
}


b2Body * PhysicsBody::GetBody() const
{
	return m_body;
}

BodyType PhysicsBody::GetBodyType() const
{
	//What kinda physics body is this?
	return m_bodyType;
}


float PhysicsBody::GetMass() const
{
	b2MassData massData;
	m_body->GetMassData(&massData);
	return massData.mass;
}

b2Vec2 PhysicsBody::GetPosition() const
{
	return m_position;
}

vec3 PhysicsBody::GetVelocity() const
{
	//Returns current velocity
	b2Vec2 vel = m_body->GetLinearVelocity();
	vec3 temp = vec3(vel.x, vel.y, 0.f);
	
	return temp;
}

float PhysicsBody::GetGravityScale() const
{
	return m_body->GetGravityScale();
}

vec4 PhysicsBody::GetColor() const
{
	return m_color;
}


vec2 PhysicsBody::GetCenterOffset() const
{
	//return offset from the center of the sprite
	return m_centerOffset;
}



float PhysicsBody::GetWidth() const
{
	//return body width
	return m_width;
}

float PhysicsBody::GetHeight() const
{
	//return body height
	return m_height;
}

float PhysicsBody::GetRadius(int fixture) const
{
	return m_body->GetFixtureList()[fixture].GetShape()->m_radius;
}


float PhysicsBody::GetRotationAngleDeg() const
{
	return Transform::ToDegrees(m_body->GetAngle());
}

bool PhysicsBody::GetFixedRotation() const
{
	return m_fixedRotation;
}


bool PhysicsBody::GetDraw()
{
	return m_drawBodies;
}

float PhysicsBody::GetHealth()
{
	return health;
}


float PhysicsBody::GetMaxHealth()
{
	return maxHealth;
}

bool PhysicsBody::GetExists()
{
	return batExists;
}


std::string PhysicsBody::GetName()
{
	return name;
}

b2Vec2 PhysicsBody::GetNextMovement()
{
	return nextMovement;

}

float PhysicsBody::GetSpeed()
{
	return speed;

}
void PhysicsBody::SetBody(b2Body * body)
{
	m_body = body;
}

void PhysicsBody::SetBodyType(BodyType type)
{
	//Set the type of body this is
	m_bodyType = type;
}


void PhysicsBody::SetPosition(b2Vec2 bodyPos, bool contactStep)
{
	if (!contactStep)
	{
		//Body transform
		m_body->SetTransform(bodyPos, m_body->GetAngle());
	}
	else
	{
		moveLater = true;
		moveValue = bodyPos;
	}
}

void PhysicsBody::SetVelocity(vec3 velo)
{
	b2Vec2 vel = b2Vec2(velo.x, velo.y);

	m_body->SetLinearVelocity(vel);
}

void PhysicsBody::SetGravityScale(float gravSc)
{
	m_body->SetGravityScale(gravSc);
}

void PhysicsBody::SetColor(vec4 col)
{
	m_color = col;
}

void PhysicsBody::SetMass(float mass)
{
	b2MassData massData;
	m_body->GetMassData(&massData);
	massData.mass = mass;

	m_body->SetMassData(&massData);
}

void PhysicsBody::SetCenterOffset(vec2 cent)
{
	//Set the center offset
	m_centerOffset = cent;
}



void PhysicsBody::ScaleBody(float scale, int fixture, bool contactStep)
{
	if (!contactStep)
	{
		if (m_bodyType == BodyType::CIRCLE)
		{
			float scaledRadius = (m_width / 2.f) * scale;

			m_width += scaledRadius;
			m_height += scaledRadius;
			m_body->GetFixtureList()[fixture].GetShape()->m_radius = m_width / 2.f;
			m_body->SetAwake(true);
		}
		else
		{
			//Used to calculate new width and height
			float lX = 999.f;
			float rX = -999.f;
			float bY = 999.f;
			float tY = -999.f;

			//Gets the shape value and casts as b2PolygonShape so we can access the vertices
			b2PolygonShape* bodyShape = (b2PolygonShape*)m_body->GetFixtureList()[fixture].GetShape();

			//Center of the polygon
			b2Vec2 center = bodyShape->m_centroid;

			//loops through every vertice
			for (int i = 0; i < bodyShape->m_count; i++)
			{
				//Create normalized direction
				b2Vec2 vert = bodyShape->m_vertices[i];
				lX = std::min(lX, vert.x);
				rX = std::max(rX, vert.x);
				bY = std::min(bY, vert.y);
				tY = std::max(tY, vert.y);

				b2Vec2 dir = (vert - center);
				dir.Normalize();

				//Moves the vert out by a scaled direction vector
				bodyShape->m_vertices[i] += scale * dir;
			}

			m_width = rX - lX;
			m_height = tY - bY;
			m_body->SetAwake(true);
		}
		
		int ent = (int)m_body->GetUserData();

		auto tempSpr = ECS::m_reg->try_get<Sprite>(ent);
		if (tempSpr != nullptr)
		{
			tempSpr->SetWidth(m_width);
			tempSpr->SetHeight(m_height);
		}
	}
	else
	{
		scaleLater = true;
		scaleFixt = fixture;
		scaleVal = scale;
	}
}

void PhysicsBody::ScaleBodyWidth(float scale, int fixture, bool contactStep)
{
	if (!contactStep)
	{
		if (m_bodyType == BodyType::CIRCLE)
		{
			float scaledRadius = (m_width / 2.f) * scale;

			m_width += scaledRadius;
			
			m_body->GetFixtureList()[fixture].GetShape()->m_radius = m_width / 2.f;
			m_body->SetAwake(true);
		}
		else
		{
			//Used to calculate new width and height
			float lX = 999.f;
			float rX = -999.f;
			float bY = 999.f;
			float tY = -999.f;

			//Gets the shape value and casts as b2PolygonShape so we can access the vertices
			b2PolygonShape* bodyShape = (b2PolygonShape*)m_body->GetFixtureList()[fixture].GetShape();

			//Center of the polygon
			b2Vec2 center = bodyShape->m_centroid;

			//loops through every vertice
			for (int i = 0; i < bodyShape->m_count; i++)
			{
				//Create normalized direction
				b2Vec2 vert = bodyShape->m_vertices[i];
				lX = std::min(lX, vert.x);
				rX = std::max(rX, vert.x);
				bY = std::min(bY, vert.y);
				tY = std::max(tY, vert.y);

				b2Vec2 dir = (vert - center);
				dir.Normalize();

				//Moves the vert out by a scaled direction vector
				bodyShape->m_vertices[i].x += scale * dir.x;
			}

			m_width = rX - lX;
			
			m_body->SetAwake(true);
		}

		int ent = (int)m_body->GetUserData();

		auto tempSpr = ECS::m_reg->try_get<Sprite>(ent);
		if (tempSpr != nullptr)
		{
			tempSpr->SetWidth(m_width);
			//tempSpr->SetHeight(m_height);
		}
	}
	else
	{
		scaleLater = true;
		scaleFixt = fixture;
		scaleVal = scale;
	}
}

void PhysicsBody::ScaleBodyHeight(float scale, int fixture, bool contactStep)
{
	if (!contactStep)
	{
		if (m_bodyType == BodyType::CIRCLE)
		{
			float scaledRadius = (m_width / 2.f) * scale;

			
			m_height += scaledRadius;
			m_body->GetFixtureList()[fixture].GetShape()->m_radius = m_width / 2.f;
			m_body->SetAwake(true);
		}
		else
		{
			//Used to calculate new width and height
			float lX = 999.f;
			float rX = -999.f;
			float bY = 999.f;
			float tY = -999.f;

			//Gets the shape value and casts as b2PolygonShape so we can access the vertices
			b2PolygonShape* bodyShape = (b2PolygonShape*)m_body->GetFixtureList()[fixture].GetShape();

			//Center of the polygon
			b2Vec2 center = bodyShape->m_centroid;

			//loops through every vertice
			for (int i = 0; i < bodyShape->m_count; i++)
			{
				//Create normalized direction
				b2Vec2 vert = bodyShape->m_vertices[i];
				lX = std::min(lX, vert.x);
				rX = std::max(rX, vert.x);
				bY = std::min(bY, vert.y);
				tY = std::max(tY, vert.y);

				b2Vec2 dir = (vert - center);
				dir.Normalize();

				//Moves the vert out by a scaled direction vector
				bodyShape->m_vertices[i].y += scale * dir.y;
			}

			m_height = tY - bY;

			
			m_body->SetAwake(true);
		}

		int ent = (int)m_body->GetUserData();

		auto tempSpr = ECS::m_reg->try_get<Sprite>(ent);
		if (tempSpr != nullptr)
		{
			//tempSpr->SetWidth(m_width);
			tempSpr->SetHeight(m_height);
		}
	}
	else
	{
		scaleLater = true;
		scaleFixt = fixture;
		scaleVal = scale;
	}
}

void PhysicsBody::SetRotationAngleDeg(float degrees, bool contactStep)
{
	if (!contactStep)
	{
		//Set the rotation angle
		m_body->SetTransform(m_body->GetPosition(), Transform::ToRadians(degrees));
	}
	else
	{
		rotateLater = true;
		rotationDeg = degrees;
	}
}

void PhysicsBody::SetFixedRotation(bool fixed)
{
	m_body->SetFixedRotation(fixed);

	m_fixedRotation = fixed;
}

void PhysicsBody::SetCategoryBit(EntityCategories category, int fixture)
{
	b2Filter filter = m_body->GetFixtureList()[fixture].GetFilterData();

	filter.categoryBits = category;

	m_body->GetFixtureList()[fixture].SetFilterData(filter);
}

void PhysicsBody::SetCollisionBit(EntityCategories collision, int fixture)
{
	b2Filter filter = m_body->GetFixtureList()[fixture].GetFilterData();

	filter.maskBits = collision;

	m_body->GetFixtureList()[fixture].SetFilterData(filter);
}


void PhysicsBody::SetDraw(bool drawBodies)
{
	m_drawBodies = drawBodies;
}

void PhysicsBody::SetHealth(float hp)
{
	if (hp >= 0)
	{
		health = hp;
	}
}

void PhysicsBody::SetName(std::string n)
{
	name = n;
}

void PhysicsBody::TakeDamage(float dmg,int ent)
{

		health -= dmg;

		if (health <= 0)
		{
			if (GetName() != "Boss")
			{
				health = 0;
				std::cout << "\nEnemy Killed";

				batExists = false;

				//ECS::GetComponent<Transform>(ent).SetPositionX(-10000.f);

				//SetPosition(b2Vec2(-10000,0), false);

				m_bodiesToDelete.push_back(ent);
			}

		}
	
}
b2Vec2 PhysicsBody::CalculateMovement(b2Vec2 target)
{
	
	b2Vec2 direction = target - GetPosition();
	float distance = sqrt((direction.x * direction.x) + (direction.y * direction.y));
	direction = b2Vec2(direction.x / distance, direction.y / distance);
	//direction = b2Vec2(direction.x, direction.y);
	return direction;
	

}
void PhysicsBody::dispatchAI()
{

		findStartAndEnd(GetPosition(), ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition());
		CalculatePath();
		SetGravityScale(0);

}

void PhysicsBody::SetNextMovement(b2Vec2 pos)
{
	nextMovement = pos;

}

void PhysicsBody::SetSpeed(float sp)
{
	speed = sp;

}

void PhysicsBody::isKnocked()
{
	knockedBack = true;
}

void PhysicsBody::SetMaxHealth(float hp)
{

		maxHealth = hp;
	
}


void PhysicsBody::SetHealthBar(int obj)
{

	healthBar = obj;

}

void PhysicsBody::SetAgro(bool hi)
{
	playerSpotted = hi;

}