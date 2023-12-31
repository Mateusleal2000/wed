#include "Wed.hpp"
#include <iostream>


Wed::Wed(pair<int, int> _edge): edge(_edge) {
    start = nullptr;
    end = nullptr;
    left = nullptr;
    right = nullptr;
    left_prev = nullptr;
    left_next = nullptr;
    right_prev = nullptr;
    right_next = nullptr;
    status = STATUS::NOT_CREATED;
    visit = false;
}

Wed::~Wed(){}

void Wed::rightNext(std::unordered_multimap<pair<int,int>, glm::vec3, KeyHasher> *edge_face_map,
                    std::unordered_multimap<pair<int,int>, Wed*, KeyHasher> *edge_creation_map, 
                    std::unordered_multimap<glm::vec3, Face*, KeyHasher2> *face_map,
                    std::vector<std::pair<glm::vec3, Vertex*>> *vertex_vector) {
    

    // Primeiro passo:
    // Verificar se já foi encontrado um right next previamente
    // Descobrir a face qual é o próximo
    auto it = edge_face_map->find(edge);
    
    if((it)==edge_face_map->end()){
        std::cout<<"meu mano era end()\n";
    }
    glm::vec3 face_right = (*it).second;
    pair<int,int> par_right = (*it).first;
    
    glm::vec3 face_left = glm::vec3(0.);
    pair<int,int> par_left(-1, -1);
    // std::cout<<"testando 1\n";
    if((++it )!=edge_face_map->end()){
        // std::cout<<"testando 2\n";
        face_left = (*it).second;
        par_left = (*it).first;
    }


    pair<int, int> next_pair(-10,-10);
    // se tiver invertido, troca-se as faces
    if(edge.first == par_right.second && edge.second == par_right.first) {
        pair<int,int> tempParRight = par_right;
        glm::vec3 tempFaceRight = face_right;
        par_right = par_left;
        face_right = face_left;
        par_left = tempParRight;
        face_left = tempFaceRight;
    }

    //calculo de next:
    if(par_right.first == face_right.x){
        next_pair.first=face_right.y;
        next_pair.second=face_right.z;
    }
    else if(par_right.first == face_right.y){
        next_pair.first=face_right.z;
        next_pair.second=face_right.x;
    }
    else if(par_right.first == face_right.z){
        next_pair.first=face_right.x;
        next_pair.second=face_right.y;
    }

    auto face_pair = checkFaceMap(face_right, face_left, face_map);
    right = face_pair.first;
    left = face_pair.second;
    end = checkVertexVector(edge.second, vertex_vector);

    // Segundo Passo:
    // Achado a aresta rn, alteramos seus parâmetros corretamente

    Wed *rn = checkWedMap(next_pair, edge_creation_map, vertex_vector);
    this->right_next = rn;

    // Terceiro Passo
    // Chamar a recursão do rn
    if (rn->right_next == nullptr)
    {
        rn->rightNext(edge_face_map, edge_creation_map, face_map, vertex_vector);
        rn->leftNext(edge_face_map, edge_creation_map, face_map, vertex_vector);
    }
}



void Wed::leftNext(std::unordered_multimap<pair<int,int>, glm::vec3, KeyHasher> *edge_face_map,
                    std::unordered_multimap<pair<int,int>, Wed*, KeyHasher> *edge_creation_map,
                    std::unordered_multimap<glm::vec3, Face*, KeyHasher2> *face_map,
                    std::vector<std::pair<glm::vec3, Vertex*>> *vertex_vector) {
    // std::cout<<"Entrou no left\n";
    if (edge_face_map->count(edge) == 1){
        return;
    }
    auto it = edge_face_map->find(edge);

    glm::vec3 face_right = (*it).second;
    pair<int,int> par_right = (*it).first;


    glm::vec3 face_left = glm::vec3(0.);
    pair<int,int> par_left(-1, -1);
    if((++it )!=edge_face_map->end()){
        face_left = (*it).second;
        par_left = (*it).first;
    }

    pair<int, int> next_pair(0,0);
    if(edge.first == par_right.first && edge.second == par_right.second) {
        pair<int,int> tempParRight = par_right;
        glm::vec3 tempFaceRight = face_right;
        par_right = par_left;
        face_right = face_left;
        par_left = tempParRight;
        face_left = tempFaceRight;
    }

    //calculo de next:
    if(par_right.first == face_right.x){
        next_pair.first=face_right.y;
        next_pair.second=face_right.z;
    }
    else if(par_right.first == face_right.y){
        next_pair.first=face_right.z;
        next_pair.second=face_right.x;
    }
    else if(par_right.first == face_right.z){
        next_pair.first=face_right.x;
        next_pair.second=face_right.y;
    }

    auto face_pair = checkFaceMap(face_right, face_left, face_map);
    //troquei a right para second e left para first. LEMBRAR!!!
    right = face_pair.second;
    left = face_pair.first;
    end = checkVertexVector(edge.second, vertex_vector);
     
    // Segundo Passo:
    // Achado a aresta ln, alteramos seus parâmetros corretamente

    Wed *ln = checkWedMap(next_pair, edge_creation_map, vertex_vector);
    this->left_next = ln;
    
    // Terceiro Passo
    // Chamar a recursão do ln
    if (ln->right_next == nullptr)
    {
        
        ln->rightNext(edge_face_map, edge_creation_map, face_map, vertex_vector);
        
        ln->leftNext(edge_face_map, edge_creation_map, face_map, vertex_vector);
    }

    return;
}



Vertex* Wed::checkVertexVector(int index, std::vector<std::pair<glm::vec3, Vertex*>> *vertex_vector) {
    Vertex * return_vertex;
    if(vertex_vector->at(index).second != nullptr){
        return_vertex = vertex_vector->at(index).second;
    }
    else{
        return_vertex = new Vertex(index, this);
        return_vertex->color = glm::vec3(0.7,0.,0.7);
        return_vertex->pos = vertex_vector->at(index).first; 
        vertex_vector->at(index).second = return_vertex;
    }
    return return_vertex;
}

std::pair<Face*, Face*> Wed::checkFaceMap(glm::vec3 face_right, glm::vec3 face_left, 
                                        std::unordered_multimap<glm::vec3, Face*, KeyHasher2> *face_map) {
                                            
    std::pair<Face *, Face *> return_face;

    bool wasEmpty = face_map->empty();

    auto face_right_iterator = face_map->find(face_right);
    auto face_left_iterator = face_map->find(face_left);
    
    if(face_right_iterator != face_map->end()) {
        return_face.first = (*face_right_iterator).second;
    }
    else{
        return_face.first = new Face(this);
        face_map->insert({face_right, return_face.first});
    }

    // verificação para resolver o problema das faces right e left terem a mesma aresta base
    if (!wasEmpty){
        if(face_left_iterator != face_map->end()) {
            return_face.second = (*face_left_iterator).second;
        }
        else{
            if (face_left == glm::vec3(0.))
            {
                return_face.second = nullptr;
            }
            else {
                return_face.second = new Face(this);
                face_map->insert({face_left, return_face.second});
            }
        }
    }
    return return_face;
}

Wed * Wed::checkWedMap(pair<int,int> next_pair, 
                        std::unordered_multimap<pair<int,int>, Wed*, KeyHasher> *edge_creation_map,
                        std::vector<std::pair<glm::vec3, Vertex*>> *vertex_vector) {
    auto it_edge = edge_creation_map->find(next_pair);

    Wed * return_wed;

    if (it_edge != edge_creation_map->end())
    {
        return_wed = (*it_edge).second;
        // Completando volta na face
        if (return_wed->right_prev == nullptr) {
            return_wed->right_prev = this;
            if(edge.first == return_wed->edge.first || edge.first == return_wed->edge.second){
                return_wed->start = checkVertexVector(edge.first, vertex_vector);
            }
            else{
                return_wed->start = checkVertexVector(edge.second, vertex_vector);
            }
        // return_wed->start = checkVertexVector(edge.second, vertex_vector);
        }
        // return_wed ja tinha sido feita na outra face
        else {
          return_wed->left_prev = this;
        }
    }
    else {
        return_wed = new Wed(next_pair);
        return_wed->status == STATUS::CREATED;
        return_wed->right_prev = this;
        if(edge.first == return_wed->edge.first || edge.first == return_wed->edge.second){
            return_wed->start = checkVertexVector(edge.first, vertex_vector);
        }
        else{
          return_wed->start = checkVertexVector(edge.second, vertex_vector);
        }
        edge_creation_map->insert({next_pair, return_wed});
    }
    
    return return_wed;
}

QString Wed::debugWed(){
    std::string s = "<"+std::to_string(edge.first)+","+std::to_string(edge.second)+">";
    return QString::fromStdString(s);
}




