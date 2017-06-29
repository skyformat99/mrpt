/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2017, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */
#pragma once
#include <QGLWidget>

#include "mrpt/opengl/CSetOfObjects.h"
#include "mrpt/maps/CSimpleMap.h"
#include "mrpt/maps/CMultiMetricMap.h"


class CGlWidget : public QGLWidget
{
public:
	CGlWidget(QWidget* parent);
	void fillMap(const mrpt::opengl::CSetOfObjects::Ptr &renderizableMap);
	void setSelected(const mrpt::math::TPose3D &pose);

protected:
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void wheelEvent(QWheelEvent *event) override;

public:
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int width, int height) override;

	mrpt::opengl::COpenGLScene::Ptr m_3Dscene;
	QPoint m_previsionPos;
	bool m_isPressMouse;
	mrpt::opengl::CSetOfObjects::Ptr m_map;
};
