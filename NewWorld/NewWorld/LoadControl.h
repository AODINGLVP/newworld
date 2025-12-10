#include"GameObjects.h"
class LoadControl
{
public:
	string type;
	string name;
	Vec3 position;
	Vec3 rotation;
	string location;
	string texturename;

	map<string, Animatemodels> Aniname;
	map<string, Staticmodels> Staticname;
	LoadControl() {

	}

	void Loadgame(vector<Enemies>& enemies, vector<StaticModle>* staticmodles, Core* core, PSOManager* psos, Shaders* shaders,TextureManager* textures) {
		ifstream file("../Resources/loadfile1.csv");

		string line;

		getline(file, line);

		while (getline(file, line)) {
			std::stringstream theline(line);
			std::string word;

			getline(theline, word, ';');
			type = word;
			getline(theline, word, ';');
			name = word;
			getline(theline, word, ';');
			position.x = stof(word);
			getline(theline, word, ';');
			position.y = stof(word);
			getline(theline, word, ';');
			position.z = stof(word);
			getline(theline, word, ';');
			rotation.x = stof(word);
			getline(theline, word, ';');
			rotation.y = stof(word);
			getline(theline, word, ';');
			rotation.z = stof(word);
			getline(theline, word, ';');
			location = (word);
			getline(theline, word, ';');
			texturename = (word);


			if (type == "static") {
				if (name == "tree") {
					
					StaticModle tree;
					tree.load(core, location, shaders, psos, Staticmodels::Tree, position, textures, texturename);
					staticmodles->push_back(move(tree));
					

				}

			}
			else if (type == "anim") {

				if (name == "trex") {
					Enemies scv;
					enemies.push_back(scv);
					enemies[0].init(core, shaders, psos, position, textures, "enemy");
					
					
				}
				
			}

		}


		file.close();
	}

};

