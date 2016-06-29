---
layout: codesamples
title:  "Sample Code"
date:   2016-06-24 18:34:36 -0700
categories: jekyll update
---
 This is sample code from one of my student project The Cavern. 
The function is to go through the objects that were deleted last frame and delete them this frame while sending a message to anyone whose listening

{% highlight cpp %}
void Factory::Update(float dt)
{
  //Delete all objects in the ObjectsToBeDeleted list 
  std::set<Component::GOC*>::iterator it = ObjectsToBeDeleted.begin();

  for (; it != ObjectsToBeDeleted.end(); ++it)
  {
    //Null object was deleted
    if (*it == nullptr)
      continue;
    
    Component::GOC * gameObj = *it;
    GameObjectIdMapType::iterator item = GameObjectIdMap.find(gameObj->ObjectID);
    ErrorIf(item == GameObjectIdMap.end(), 
      "Object %d was double deleted or is bad memory.", gameObj->ObjectID);
    if (item != GameObjectIdMap.end())
    {
      CORE->BroadcastInstantMessage(&MessageObjectDeleted(gameObj->GetID(), gameObj));
      //Delete it and remove its entry in the ID map
      ObjList.erase(gameObj);
      delete gameObj;
      gameObj = nullptr;
      GameObjectIdMap.erase(item);
    }
  }
  //All objects to be delete have been deleted
  ObjectsToBeDeleted.clear();
}
{% endhighlight %}

This was sample code from my A.I. class where we had to implement vision in front of the player.
Results shown below

{% highlight cpp %}
float AngleBetweenVec2(Vec2& vec1, Vec2& vec2)
{
  float vec1Length = vec1.Length();
  //if for some reason this is 0, don't divide by zero
  if (vec1Length == 0.0f)
    return acos(0.0f);
  
  float vec2Length = vec2.Length();
  
  if (vec2Length == 0.0f)
    return acos(0.0f);
  
  return  180.0f / 3.1415f * acos(vec1.Dot(vec2) / (vec1Length * vec2Length));
}

void Terrain::AnalyzeSearch(void)
{
  Vec2 pos(m_cPlayer, m_rPlayer);
  Vec2 dir(m_dirPlayer.x, m_dirPlayer.z);
  int width = GetMapWidth();
  int height = GetMapHeight();
  
  //Check through grid to see if the tile is visible to the player
  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      Vec2 curPos(j, i);
      Vec2 temp = curPos - pos;
      
      if (IsWall(i, j))
        continue;
      
      //Check if the tile is visibile in front of the player
      if (AngleBetweenVec2(dir, temp) > 92.3f)
        continue;
      
      if (IsClearPath(i, j, m_rPlayer, m_cPlayer))
        m_terrainInfluenceMap[i][j] = 1.0f;
    }
  }
}
{% endhighlight %}

  ![AI Sight]({{ site.url }}/assets/visibleToPlayer.PNG)
