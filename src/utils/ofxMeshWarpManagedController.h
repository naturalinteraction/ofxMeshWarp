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

		void setPixels(ofPixels &pix) { mover_.setPixels(pix); }
		void elevationWarp(glm::vec2 my_translation, float my_scale, float drama) { mover_.elevationWarp(my_translation, my_scale, drama); }
		void setCenterOfProjection(float x, float y) { center_of_projection.x = x; center_of_projection.y = y; mover_.center_of_projection.x = x; mover_.center_of_projection.y = y; }

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
