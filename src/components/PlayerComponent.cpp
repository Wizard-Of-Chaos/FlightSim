#include "PlayerComponent.h"
#include "HUDHeader.h"

void PlayerComponent::removeHUDElement(HUDElement* element)
{
	for (unsigned int i = 0; i < HUD.size(); ++i) {
		if (HUD[i] == element) {
			HUD.erase(HUD.begin() + i);
			delete element;
			return;
		}
	}
}

void PlayerComponent::removeContact(HUDContact* contact)
{
	if (!contact) return;
	trackedContacts[contact->contact] = nullptr;
	removeHUDElement(contact);
}