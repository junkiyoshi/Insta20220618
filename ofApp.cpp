#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->frame.clear();

	int radius = 100;
	int phi_deg_step = 5;

	for (int x = -400; x <= 400; x += 400) {

		for (int z = -400; z <= 400; z += 400) {

			for (float theta_deg = 1; theta_deg < 180; theta_deg += 10) {

				auto noise_z = radius * cos(theta_deg * DEG_TO_RAD);
				auto noise_value = ofNoise(x, z, noise_z * 0.05 + ofGetFrameNum() * 0.03);

				auto param_x = ofMap(noise_value, 0, 1, radius * -1, radius * 1);

				auto angle_x = ofMap(ofNoise(x, ofGetFrameNum() * 0.002), 0, 1, -PI * 2, PI * 2);
				auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));

				auto angle_y = ofMap(ofNoise(z, ofGetFrameNum() * 0.002), 0, 1, -PI * 2, PI * 2);
				auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));

				auto start_index = this->face.getNumVertices();
				for (int phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

					auto index = this->face.getNumVertices();

					vector<glm::vec3> vertices;

					vertices.push_back(glm::vec3(x, 0, z) + glm::vec4(param_x + radius * sin((theta_deg - 5) * DEG_TO_RAD) * cos(phi_deg * DEG_TO_RAD), radius * sin((theta_deg - 5) * DEG_TO_RAD) * sin(phi_deg * DEG_TO_RAD), radius * cos((theta_deg - 5) * DEG_TO_RAD), 0) * rotation_y * rotation_x);
					vertices.push_back(glm::vec3(x, 0, z) + glm::vec4(param_x + radius * sin((theta_deg + 5) * DEG_TO_RAD) * cos(phi_deg * DEG_TO_RAD), radius * sin((theta_deg + 5) * DEG_TO_RAD) * sin(phi_deg * DEG_TO_RAD), radius * cos((theta_deg + 5) * DEG_TO_RAD), 0) * rotation_y * rotation_x);

					this->face.addVertices(vertices);
					this->frame.addVertices(vertices);

					if (phi_deg > 0) {

						this->face.addIndex(index + 0); this->face.addIndex(index - 2); this->face.addIndex(index - 1);
						this->face.addIndex(index + 0); this->face.addIndex(index - 1); this->face.addIndex(index + 1);

						this->face.addIndex(index + 0); this->face.addIndex(index - 2); this->face.addIndex(start_index + 0);
						this->face.addIndex(index + 1); this->face.addIndex(index - 1); this->face.addIndex(start_index + 1);

						this->frame.addIndex(index + 0); this->frame.addIndex(index - 2);
						this->frame.addIndex(index + 1); this->frame.addIndex(index - 1);
					}
				}

				auto index = this->face.getNumVertices() - 2;
				this->face.addIndex(start_index + 0); this->face.addIndex(index + 0); this->face.addIndex(index + 1);
				this->face.addIndex(start_index + 1); this->face.addIndex(index + 1); this->face.addIndex(start_index + 1);

				this->frame.addIndex(start_index + 0); this->frame.addIndex(index + 0);
				this->frame.addIndex(start_index + 1); this->frame.addIndex(index + 1);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(270);

	ofSetColor(0);
	this->face.draw();

	ofSetColor(255);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}