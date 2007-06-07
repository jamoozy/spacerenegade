#include "GL/glut.h"
#include "buildcomponent.h"

void Material::draw(int pass)
{
	if (pass == 1)
	{
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();

		glColor3f(1,1,1);
		glTranslated(position.x(), position.y(), position.z());
		glutSolidSphere(1,10,4);

		glPopMatrix();
	}
}

