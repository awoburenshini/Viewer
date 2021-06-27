#include <MeshIO.h>

#include <fstream>
#include <unordered_map>
#include <iostream>
#include <map>

using namespace std;



inline std::vector<std::string> &str_tokenize(const std::string &s, char delim, std::vector<std::string> &elems, bool include_empty = false) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		if (!item.empty() || include_empty)
			elems.push_back(item);
	return elems;
}

inline std::vector<std::string> str_tokenize(const std::string &s, char delim, bool include_empty) {
	std::vector<std::string> elems;
	str_tokenize(s, delim, elems, include_empty);
	return elems;
}

inline uint32_t str_to_uint32_t(const std::string &str) {
	char *end_ptr = nullptr;
	uint32_t result = (uint32_t)strtoul(str.c_str(), &end_ptr, 10);
	if (*end_ptr != '\0')
		throw std::runtime_error("Could not parse unsigned integer \"" + str + "\"");
	return result;
}

void load_obj(const std::string& filename, MatrixXf& V, MatrixXu& F)
{
	/// Vertex indices used by the OBJ format
	struct obj_vertex {
		uint32_t p = (uint32_t)-1;
		uint32_t n = (uint32_t)-1;
		uint32_t uv = (uint32_t)-1;

		inline obj_vertex() { }

		inline obj_vertex(const std::string &string) {
			std::vector<std::string> tokens = str_tokenize(string, '/', true);

			if (tokens.size() < 1 || tokens.size() > 3)
				throw std::runtime_error("Invalid vertex data: \"" + string + "\"");

			p = str_to_uint32_t(tokens[0]);

#if 0
			if (tokens.size() >= 2 && !tokens[1].empty())
				uv = str_to_uint32_t(tokens[1]);

			if (tokens.size() >= 3 && !tokens[2].empty())
				n = str_to_uint32_t(tokens[2]);
#endif
		}

		inline bool operator==(const obj_vertex &v) const {
			return v.p == p && v.n == n && v.uv == uv;
		}
	};

	/// Hash function for obj_vertex
	struct obj_vertexHash {
		std::size_t operator()(const obj_vertex &v) const {
			size_t hash = std::hash<uint32_t>()(v.p);
			hash = hash * 37 + std::hash<uint32_t>()(v.uv);
			hash = hash * 37 + std::hash<uint32_t>()(v.n);
			return hash;
		}
	};

	typedef std::unordered_map<obj_vertex, uint32_t, obj_vertexHash> VertexMap;

	std::ifstream is(filename);

	std::vector<Vector3f>   positions;
	//std::vector<Vector2d>   texcoords;
	//std::vector<Vector3d>   normals;
	std::vector<uint32_t>   indices;
	std::vector<obj_vertex> vertices;
	VertexMap vertexMap;

	std::string line_str;
	while (std::getline(is, line_str)) {
		std::istringstream line(line_str);

		std::string prefix;
		line >> prefix;

		if (prefix == "v") {
			Vector3f p;
			line >> p.x() >> p.y() >> p.z();
			positions.push_back(p);
		}
		else if (prefix == "vt") {
			/*
			Vector2d tc;
			line >> tc.x() >> tc.y();
			texcoords.push_back(tc);
			*/
		}
		else if (prefix == "vn") {
			/*
			Vector3d n;
			line >> n.x() >> n.y() >> n.z();
			normals.push_back(n);
			*/
		}
		else if (prefix == "f") {
			std::string v1, v2, v3, v4;
			line >> v1 >> v2 >> v3 >> v4;
			obj_vertex tri[6];
			int nVertices = 3;

			tri[0] = obj_vertex(v1);
			tri[1] = obj_vertex(v2);
			tri[2] = obj_vertex(v3);

			if (!v4.empty()) {
				/* This is a quad, split into two triangles */
				tri[3] = obj_vertex(v4);
				tri[4] = tri[0];
				tri[5] = tri[2];
				nVertices = 6;
			}
			/* Convert to an indexed vertex list */
			for (int i = 0; i<nVertices; ++i) {
				const obj_vertex &v = tri[i];
				VertexMap::const_iterator it = vertexMap.find(v);
				if (it == vertexMap.end()) {
					vertexMap[v] = (uint32_t)vertices.size();
					indices.push_back((uint32_t)vertices.size());
					vertices.push_back(v);
				}
				else {
					indices.push_back(it->second);
				}
			}
		}
	}

	F.resize(3, indices.size() / 3);
	memcpy(F.data(), indices.data(), sizeof(uint32_t)*indices.size());

	V.resize(3, vertices.size());
	for (uint32_t i = 0; i<vertices.size(); ++i)
		V.col(i) = positions.at(vertices[i].p - 1);
}


void write_obj(const std::string &filename, const MatrixXf &V, const MatrixXu &F,
                const MatrixXf &N, const MatrixXf &Nf,
               const MatrixXf &UV, const MatrixXf &C) {
    cout << "Writing \"" << filename << "\" (V=" << V.cols()
         << ", F=" << F.cols() << ") .. ";
    cout.flush();
    std::ofstream os(filename);
    if (os.fail())
        throw std::runtime_error("Unable to open OBJ file \"" + filename + "\"!");
    if (N.size() > 0 && Nf.size() > 0)
        throw std::runtime_error("Please specify either face or vertex normals but not both!");

    for (uint32_t i = 0; i < V.cols(); ++i)
        os << "v " << V(0, i) << " " << V(1, i) << " " << V(2, i) << endl;

    for (uint32_t i = 0; i < N.cols(); ++i)
        os << "vn " << N(0, i) << " " << N(1, i) << " " << N(2, i) << endl;

    for (uint32_t i = 0; i < Nf.cols(); ++i)
        os << "vn " << Nf(0, i) << " " << Nf(1, i) << " " << Nf(2, i) << endl;

    for (uint32_t i = 0; i < UV.cols(); ++i)
        os << "vt " << UV(0, i) << " " << UV(1, i) << endl;

    /* Check for irregular faces */
    std::map<uint32_t, std::pair<uint32_t, std::map<uint32_t, uint32_t>>> irregular;
    size_t nIrregular = 0;

    for (uint32_t f = 0; f < F.cols(); ++f) {
        if (F.rows() == 4) {
            if (F(2, f) == F(3, f)) {
                nIrregular++;
                auto &value           = irregular[F(2, f)];
                value.first           = f;
                value.second[F(0, f)] = F(1, f);
                continue;
            }
        }
        os << "f ";
        for (uint32_t j = 0; j < F.rows(); ++j) {
            uint32_t idx = F(j, f);
            idx += 1;
            os << idx;
            if (Nf.size() > 0)
                idx = f + 1;
            os << "//" << idx << " ";
        }
        os << endl;
    }

    for (auto item : irregular) {
        auto face  = item.second;
        uint32_t v = face.second.begin()->first, first = v, i = 0;
        os << "f ";
        while (true) {
            uint32_t idx = v + 1;
            os << idx;
            if (Nf.size() > 0)
                idx = face.first + 1;
            os << "//" << idx << " ";

            v = face.second[v];
            if (v == first || ++i == face.second.size())
                break;
        }
        os << endl;
    }

    cout << "done. ";
    if (irregular.size() > 0)
        cout << irregular.size() << " irregular faces, ";
}