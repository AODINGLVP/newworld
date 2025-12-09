#include"GameObjects.h"
class LoadControl
{
public:
	string type;
	string name;
	Vec3 position;
	Vec3 rotation;


	map<string, Animatemodels> Aniname;
	map<string, Staticmodels> Staticname;
	LoadControl() {

	}

	void Loadgame(vector<Enemies>* enemies, vector<StaticModle>* staticmodles, Core* core, PSOManager* psos, Shaders* shaders,TextureManager* textures) {
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


			if (type == "static") {
				if (name == "tree") {
					StaticModle tree;
					staticmodles->push_back(tree);
					staticmodles->back().load(core, "../Resources/acacia_003.gem", shaders, psos, Staticmodels::Tree, position,textures,"OtherTree");

				}

			}
			else if (type == "anim") {

				if (name == "trex") {
					Enemies enemy;
					enemy.init(core, shaders, psos, position);
					enemies->push_back(enemy);
				}
			}

		}


		file.close();
	}

};

