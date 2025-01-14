#pragma once

#include "ofxMeshWarpController.h"
#include "ofEvents.h"

namespace ofx{namespace MeshWarp{
namespace Editor {
	class ManagedController : public ControllerBase {
	public:
		void add(std::shared_ptr<Mesh> target);
		void clear();
		void clearOperation();

		void setUVResolution(const glm::vec2 &res) { mover_.setUVResolution(res); }

		void setElevationPixels(ofPixels &pix) { mover_.setElevationPixels(pix); }
		void elevationWarp(glm::vec2 my_translation, float my_scale, float drama, float my_rotation) { mover_.elevationWarp(my_translation, my_scale, drama, my_rotation); }
		void setCenterOfProjection(float x, float y) { center_of_projection.x = x; center_of_projection.y = y; mover_.center_of_projection.x = x; mover_.center_of_projection.y = y; }
		void setRotation(float r) { rotation_ = r; mover_.rotation_ = r; }

		void setAnchorPoint(float x, float y);
		void setTranslation(float x, float y);
		void setScale(float s);

		void drawCustom() const;

		void mousePressed(ofMouseEventArgs &args);
		void mouseReleased(ofMouseEventArgs &args);
		void mouseMoved(ofMouseEventArgs &args);
		void mouseDragged(ofMouseEventArgs &args);
		void mouseScrolled(ofMouseEventArgs &args);
		void mouseEntered(ofMouseEventArgs &args);
		void mouseExited(ofMouseEventArgs &args);
		void keyPressed(ofKeyEventArgs &args);
		void keyReleased(ofKeyEventArgs &args);

		float x_scale = 1.0;
		float drama = 0.0;
		float my_scale = 0.0;
		float my_rotation = 0.0;
		glm::vec2 my_translation;

	private:
		bool is_mover_active_ = false;
		bool is_divider_active_ = false;
		PointController mover_;
		DivideController divider_;
		std::vector<ofxMeshWarpControllerBase*> controllers_{&mover_,&divider_};
		void activateMover();
		void activateDivider();
	};
}
}}
using ofxMeshWarpController = ofx::MeshWarp::Editor::ManagedController;
