//
// Created by Tony Adriansen on 3/2/24.
//

#include "Group.hpp"
void Group::AddObject(std::unique_ptr<Object> object) {
  m_objects.push_back(std::move(object));
}

void Group::RemoveObject(const Object* object) {
  int i = 0;
  for (auto& o : m_objects) {
    if (o.get() == object) {
      m_objects.erase(m_objects.begin() + i);
      break;
    }
    i++;
  }
}