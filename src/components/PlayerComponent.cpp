#include "PlayerComponent.h"
#include "HUDHeader.h"

void PlayerComponent::removeHUDElement(HUDElement* element)
{
	for (unsigned int i = 0; i < HUD.size(); ++i) {
		if (HUD[i] == element) {
			HUD.erase(HUD.begin() + i);
			delete element; //The delete function for a HUD element should be written to appropriately kill all nodes
			return;
		}
	}
}

void PlayerComponent::removeContact(HUDContact* contact)
{
	if (!contact) return;
	for (auto [id, hud] : trackedContacts) {
		if (id != contact->contact) continue;
		trackedContacts[id] = nullptr;
		removeHUDElement(contact);
		return;
	}
}