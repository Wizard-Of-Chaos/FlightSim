#include "PlayerComponent.h"
#include "HUDHeader.h"

void PlayerComponent::removeHUDElement(HUDElement* element)
{
	for (unsigned int i = 0; i < HUD.size(); ++i) {
		if (HUD[i] == element) {
			HUD.erase(HUD.begin() + i);
			element->elem->remove();
			delete element;
			return;
		}
	}
}

void PlayerComponent::removeContact(HUDContact* contact)
{
	if (!contact) return;
	contact->marker->remove();
	trackedContacts[contact->contact] = nullptr;
	removeHUDElement(contact);
}