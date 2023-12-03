#include "MyGLWidget.hpp"
#include <iostream>

using qtime = QDateTime;


MyGLWidget::MyGLWidget(QWidget * parent) : QOpenGLWidget(parent){
    setFocusPolicy(Qt::TabFocus);
    setFixedSize(900,600);
}

MyGLWidget::~MyGLWidget(){
    
    //f->glDeleteBuffers(1,&pointsVBO);
}

void MyGLWidget::initializeGL() {

    //f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(context());
    f->initializeOpenGLFunctions();
    f->glEnable(GL_DEPTH_TEST);
	f->glEnable(GL_DEPTH_CLAMP);
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	f->glEnable(GL_LINE_SMOOTH);
	f->glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    f->glEnable(GL_PROGRAM_POINT_SIZE);
    f->glDisable(GL_CULL_FACE);
    f->glPointSize(2);
    f->glLineWidth(1);



    mesh = new Mesh(context());
    
    mesh->getMeshProperties("orisso.obj");
    std::cout<<"20\n";
    mesh->init();
    std::cout<<"30\n";
    mesh->buildMesh();
    std::cout<<"40\n";
    mesh->createWedVector();
    mesh->createFaceVector();
    
    camera = new Camera(glm::vec3(0,0.,5.),glm::vec3(0.,0.,-1.));
    camera->setFov(45);
    camera->setViewPlanes(0.01,100.);
    camera->setCanvasDimensions(this->width(),this->height());

    GLProgram p0 = mesh->getProgram(0);
    uint p0ID = p0.getProgramId();
    f->glUseProgram(p0ID);
    GLuint vmatrix = f->glGetUniformLocation(p0ID, "m_view");
    GLuint pmatrix = f->glGetUniformLocation(p0ID, "m_proj");
    f->glUniformMatrix4fv(vmatrix, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
    f->glUniformMatrix4fv(pmatrix, 1, GL_FALSE, glm::value_ptr(camera->getProjMatrix()));


    f->glClearColor(45.0/255., 63/255., 133/255.,1.0f);

}

void MyGLWidget::paintGL(){
  
    mesh->draw();
}


void MyGLWidget::resizeGL(int w, int h){


}

void MyGLWidget::initTimer(){
}


void MyGLWidget::updateScene(){

    makeCurrent();

    doneCurrent();
}

bool MyGLWidget::event(QEvent *event)
{

	if (event->type() == QEvent::KeyPress)
	{

		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_P)
		{
            std::cout<< "Paused\n";
            //timer.stop();
        }

        if (keyEvent->key() == Qt::Key_S)
		{
            std::cout<<"Started\n";
            //timer.start();
        }

        if(keyEvent->key() == Qt::Key_R){
            std::cout<<"Reseted\n";
        }
    }




	return QOpenGLWidget::event(event);

}