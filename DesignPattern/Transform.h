#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Component.h"

class Transform : public Component
{
public:
	BEGIN_COMPONENT_BASE

	Transform(GameObject* gameObject);
	virtual ~Transform();

	virtual void onCreate();
	virtual void onDestroy();
private:
	virtual void UpdateComponent();
};

#endif // !TRANSFORM_H_