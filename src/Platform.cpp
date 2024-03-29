#include "Platform.h"
#include "Random.h"
#include "Log.h"

Platform::Platform()
	: m_PlatformBody(nullptr)
	, m_PlatformCount(0)
{
}

void Platform::Create(b2World &world, int platformCount)
{
	BL_ASSERT(!m_PlatformBody, "The platform has already been created !");

	if (!m_PlatformBody)
	{
		m_PlatformCount = platformCount;

		b2BodyDef def;
		def.type = b2_staticBody;
		def.position = {-50.0f, -20.0f};

		m_PlatformBody = world.CreateBody(&def);

		float angle = 0.0f, prevAngle = 0.0f, x = 0.0f, y = 0.0f;

		for (int i = 0; i < m_PlatformCount; i++)
		{
			angle = 1.05f * Random::Float(-0.5f, 0.5f) * std::powf(
				2.0f, static_cast<float>(i) / static_cast<float>(m_PlatformCount)
			);

			x += 10.0f * (cos(prevAngle) + sin(3.14159f / 2.0f - angle));
			y += 10.0f * (cos(3.14159f / 2.0f - angle) + sin(prevAngle));

			prevAngle = angle;

			b2Vec2 p1 = {-5.0f * std::cos(angle) - 1.0f * std::sin(angle) + x / 2.0f, -5.0f * std::sin(angle) + 1.0f * std::cos(angle) + y / 2.0f};
			b2Vec2 p2 = {-5.0f * std::cos(angle) + 1.0f * std::sin(angle) + x / 2.0f, -5.0f * std::sin(angle) - 1.0f * std::cos(angle) + y / 2.0f};
			b2Vec2 p3 = {5.0f * std::cos(angle) + 1.0f * std::sin(angle) + x / 2.0f, 5.0f * std::sin(angle) - 1.0f * std::cos(angle) + y / 2.0f};
			b2Vec2 p4 = {5.0f * std::cos(angle) - 1.0f * std::sin(angle) + x / 2.0f, 5.0f * std::sin(angle) + 1.0f * std::cos(angle) + y / 2.0f};

			b2Vec2 points[] = {p1, p2, p3, p4};

			b2PolygonShape shape;
			shape.Set(points, 4);

			b2FixtureDef fixture;
			fixture.shape = &shape;
			fixture.density = 1.0f;
			fixture.friction = 1.0f;
			fixture.restitution = 0.1f;

			m_PlatformBody->CreateFixture(&fixture);
		}
	}
}

void Platform::Destory()
{
	BL_ASSERT(m_PlatformBody, "The platform has not been created !");

	if (m_PlatformBody)
	{
		m_PlatformBody->GetWorld()->DestroyBody(m_PlatformBody);
	}
}

void Platform::Draw()
{
	if (m_PlatformBody)
	{
		b2Vec2 pos = m_PlatformBody->GetPosition();
		float rot = m_PlatformBody->GetAngle();

		for (b2Fixture *f = m_PlatformBody->GetFixtureList(); f; f = f->GetNext())
		{
			auto vertexArray = ((b2PolygonShape *)f->GetShape())->m_vertices;
			auto vertexCount = ((b2PolygonShape *)f->GetShape())->m_count;

			std::vector<glm::vec3> vertices;
			glm::vec4 colour = {0.8f, 0.2, 0.2f, 1.0f};

			for (int i = 0; i < vertexCount; i++)
			{
				float x = (vertexArray[i].x * std::cosf(rot) - vertexArray[i].y * std::sinf(rot)) + pos.x;
				float y = (vertexArray[i].x * std::sinf(rot) + vertexArray[i].y * std::cosf(rot)) + pos.y;

				vertices.push_back({x, y, 0.0f});
			}

			Renderer::SubmitFilledPolygon(vertices, colour);
		}
	}
}
