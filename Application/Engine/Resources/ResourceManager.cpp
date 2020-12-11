#include "pch.h"
#include "ResourceManager.h"

bool sn::ResourceManager::Startup()
{
	return true;
}

void sn::ResourceManager::Shutdown()
{
	RemoveAll();
}

void sn::ResourceManager::Update()
{

}

void sn::ResourceManager::RemoveAll()
{
	for (auto resource : m_resources) {
		resource.second->Destroy();
		delete resource.second;
	}

	m_resources.clear();
}
