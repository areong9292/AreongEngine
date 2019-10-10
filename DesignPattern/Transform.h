#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Component.h"

class Transform : public Component
{
public:
	Transform(GameObject* gameObject);
	virtual ~Transform();

	virtual void onCreate();
	virtual void onDestroy();
private:
	virtual void UpdateComponent();
};

#endif // !TRANSFORM_H_