#include"GameObjects.h"
struct Objectload
	{
		string type;
		string name;

		Vec3 position;
		Vec3 rotation;

		string location;
		string textureName;
		int iscollider;
		string meshname;
	};
class LoadControl
{
	
public:
	string type;
	string name;
	Vec3 position;
	Vec3 rotation;
	string location;
	string texturename;
	vector<Matrix> temporary;
	vector<Objectload> instance;
	
	vector<Objectload> scv;
	string meshname;
	map<string, Animatemodels> Aniname;
	map<string, Staticmodels> Staticname;
	LoadControl() {

	}

	vector<Objectload> LoadData(vector<Objectload>* scv1, vector<Objectload>* scv2) {
		ifstream file("../Resources/loadfile1.csv");

		string line;

		getline(file, line);
		Objectload getinformation;
		while (getline(file, line)) {
			std::stringstream theline(line);
			std::string word;

			getline(theline, word, ';');
			getinformation.type = word;
			getline(theline, word, ';');
			getinformation.name = word;
			getline(theline, word, ';');
			getinformation.position.x = stof(word);
			getline(theline, word, ';');
			getinformation.position.y = stof(word);
			getline(theline, word, ';');
			getinformation.position.z = stof(word);
			getline(theline, word, ';');
			getinformation.rotation.x = stof(word);
			getline(theline, word, ';');
			getinformation.rotation.y = stof(word);
			getline(theline, word, ';');
			getinformation.rotation.z = stof(word);
			getline(theline, word, ';');
			getinformation.location = (word);
			getline(theline, word, ';');
			getinformation.textureName = (word);
			getline(theline, word, ';');
			getinformation.iscollider = stoi(word);
			getline(theline, word, ';');
			getinformation.meshname = (word);
			if (getinformation.type == "static") {
				scv1->push_back(getinformation);
			}
			else {
				scv2->push_back(getinformation);
			}
			

			
		}


		file.close();
		return scv;
	}

	vector<Objectload> LoadinstaceData(vector<Objectload>* scv1) {
		ifstream file("../Resources/loadfileinstancedata.csv");

		string line;

		getline(file, line);
		Objectload getinformation;
		while (getline(file, line)) {
			std::stringstream theline(line);
			std::string word;

			getline(theline, word, ';');
			getinformation.type = word;
			getline(theline, word, ';');
			getinformation.name = word;
			getline(theline, word, ';');
			getinformation.position.x = stof(word);
			getline(theline, word, ';');
			getinformation.position.y = stof(word);
			getline(theline, word, ';');
			getinformation.position.z = stof(word);
			getline(theline, word, ';');
			getinformation.rotation.x = stof(word);
			getline(theline, word, ';');
			getinformation.rotation.y = stof(word);
			getline(theline, word, ';');
			getinformation.rotation.z = stof(word);
			getline(theline, word, ';');
			getinformation.location = (word);
			getline(theline, word, ';');
			getinformation.textureName = (word);
			getline(theline, word, ';');
			getinformation.iscollider = stoi(word);
			getline(theline, word, ';');
			getinformation.meshname = (word);
			
				scv1->push_back(getinformation);
			
			



		}


		file.close();
		return scv;
	}
	vector<Objectload> LoadinstacepositionData(vector<vector<Vec3>>* scv1) {
		ifstream file("../Resources/loadfileinstanceposition.csv");
		vector<Vec3>instances;
		string line;
		Vec3 instanceposition;
		int end;
		getline(file, line);
		Objectload getinformation;
		while (getline(file, line)) {
			std::stringstream theline(line);
			std::string word;

			getline(theline, word, ';');
			instanceposition.x = stof(word);
			getline(theline, word, ';');
			instanceposition.y = stof(word);
			getline(theline, word, ';');
			instanceposition.z = stof(word);
			getline(theline, word, ';');
			end = stoi(word);
			if (end == 1) {
				instances.push_back(instanceposition); 
					scv1->push_back(instances);
				instances.clear();
			}
			else {
				instances.push_back(instanceposition);
			}
			

		}


		file.close();
		return scv;
	}


	void Loadgame(vector<Enemies>* enemies, vector<StaticModle>* staticmodles, Core* core, PSOManager* psos, Shaders* shaders,TextureManager* textures) {
		//Actually, I'm not quite sure. Maybe it's because of the initialization of the animation model,
		// or the data passed into the vector is incorrect. In general, if I store the enemies into the vector and initialize it here, 
		// it will cause the queue of the vector to crash and report an error. The content of the error seems to be that the red-black tree was damaged due to out-of-bounds access.
		ifstream file("../Resources/loadfile1.csv");
		int iscollider;
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
			getline(theline, word, ';');
			iscollider = stoi(word);
			getline(theline, word, ';');
			meshname = word;

			if (type == "static") {
				if (name == "tree") {
					
					StaticModle tree;
					tree.load(core, location, shaders, psos, Staticmodels::Tree, position, textures, texturename, iscollider, meshname);
					staticmodles->push_back(move(tree));
					

				}

			}
			else if (type == "anim") {

				if (name == "trex") {
					Enemies scv;
					enemies->push_back(scv);
					
					
					
				}
				(*enemies)[0].init(core, shaders, psos, position, textures, "enemy",meshname);
			}

		}


		file.close();
	}

};

