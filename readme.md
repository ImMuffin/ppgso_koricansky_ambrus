# Projekt PPGSO

Autori: Adam Koričanský a Ondrej Ambruš

Team: Team 11

Téma: Akvárium

---
## Originálny popis návrhu
Letíme v priestore, kým nenarazíme na akvárium.
![Img1](./readme%20images/obrazok1.jpg)
---
Presúvame sa do akvária, ske sledujeme pohyb ryby.
![Img2](./readme%20images/obrazok2.jpg)
---
Po nájdení hradu ho rybka skúma bližšie.
![Img3](./readme%20images/obrazok3.jpg)
---
V hrade nájde rybka kamaráta ktorého začne nahánať.
![Img4](./readme%20images/obrazok4.jpg)
---
Rybky spolu nájdu truhlu s pokadom a začnú okolo nej krúžiť. Truhla sa otvorí a vyplávajú z nej bublinky.
![Img5](./readme%20images/obrazok5.jpg)
---
Truhla vtiahne rybky a animácia sa končí. V pozadí sa zablisne.
![Img6](./readme%20images/obrazok6.jpg)

## Aktuálny návrh
Počas implementácie sme sa pokúšali držať plánu. V rámci naších technických zdatností sme museli originálny návrh upraviť.

V prvej scéne krúžime okolo akvárka a sledujeme, ako sa napĺňa vodou.
![Img1.1](./readme%20images/obrazoknovy1.png)
---
Druhá scéna začína v akvárku, kde sledujeme pohyb ryby.
![Img1.2](./readme%20images/obrazoknovy2.jpg)
---
Po preskúmaní akvárka, ryba dopláva k hradu.
![Img1.3](./readme%20images/obrazoknovy3.jpg)
---
Pri hrade nájde rybka kamaráta.
![Img1.4](./readme%20images/obrazoknovy4.jpg)
---
Rybky spolu plávajú, počas čoho nájdu truhlu z ktorej vychádzajú bublinky.
![Img1.5](./readme%20images/obrazoknovy5.jpg)
---
## Implementacia

### Ovládanie:
B: Nahravanie pohybu

N: Zastavenie nahravania

M: Prehravanie

R: Prva scena

T: Druha scena

Y: Bublinky

H: Bez bubliniek

U: Napojit kameru

I: Odpojit kameru

### Popis hierarchického pohybu
Poloha objektov nachádzajúcich sa v akváriu as odvíja od štartovacieho bloku akvária (spodku akvária). Túto hierarchiu sme dosiali násobením matíc `modelMatrix`.

![Img1.5](./readme%20images/hierarchickypohyb.jpg)
``` c++
if (obj->master) //set object for connected motion
{
    masterObject = obj;
}

if ((masterObject != nullptr) && (obj->slave == true)) //connected motion
{
    glm::mat4 originalModel = obj->modelMatrix;
    obj->modelMatrix = masterObject->modelMatrix;
    obj->modelMatrix *= originalModel;
}
```

Časť hierarchie je dosiahnutá naviazaním pohybu kamery na pohyb hráča pomocou premennej `cameraFocus`

``` c++
if (obj->cameraFocus == true)
{
    camera->back = obj->position;
    camera->position = obj->position + obj->forward * 2.0f;
}
```

---
Co je dopredu?
``` c++
forward.x = sin(rotation.z)*sin(rotation.x);
forward.y = cos(rotation.x);
forward.z = sin(rotation.x)*cos(rotation.z);
```

Pretocenie kamery cez 180°.
``` c++
if (fmod(obj->rotation.x, ppgso::PI*2) - ppgso::PI < 0)
{
    camera->up.y = 1;
}
else
{
    camera->up.y = -1;
}
camera->update();
```

Vodny shader
``` c++
vec2 texcoord = texCoord;
texcoord.x += sin(texcoord.y * 4*2*3.14159 + TimeOffset) / 100;
```

Priesvitnost a rozmazanost vo vode
``` c++
shader->setUniform("Transparency",1.0f);
shader->setUniform("TimeOffset", (glfwGetTime() * 2*3.14159 * .75));
```

Bublinky sa spoja ked sa zrazia
``` C++
if (col < 0 && dis != 0) {
            auto ratio = -col / dis;
            auto vec = pdif * ratio;

            float volume1 = 4/3 * scale.x * scale.x * scale.x * 3.14;
            float volume2 = 4/3 * object->scale.x * object->scale.x * object->scale.x * 3.14;
            float finalScale = (volume1 + volume2) / (4/3 * 3.14);
            finalScale = cbrt(finalScale);
            float pos_offset = scale.x / object->scale.x;
            if (position.y > object->position.y)
            {
                scale = glm::vec3 {finalScale,finalScale,finalScale};
                object->scale = {0,0,0};
            } else {
                object->scale = glm::vec3 {finalScale,finalScale,finalScale};
                scale = {0,0,0};
            }
        }

```

Presuvanie objektov na zaklade velkosti bounding boxov
``` c++
void Scene::redistributeObjects()
{
  auto i = std::begin(objects);
  while (i != std::end(objects)) {
    auto obj = i->get();
    if(obj->redist == true)
    {
      bool distribute = true;
      while (distribute)
      {
        distribute = false;
        obj->position.x = fmod(rand(), 4.2) - 2.1;
        obj->position.z = fmod(rand(), 8.4) - 4.2;
        printf("%f, %f\n",obj->position.x, obj->position.y);
        auto otherObjects = std::begin(objects);
        while (otherObjects != std::end(objects))
        {
          auto oObj = otherObjects->get();
          if ((fabs(obj->position.x - oObj->position.x) < (obj->size.x + oObj->size.x)) && (fabs(obj->position.y - oObj->position.y) < (obj->size.y + oObj->size.y)) && (fabs(obj->position.z - oObj->position.z) < (obj->size.z + oObj->size.z)) && oObj != obj)
          {
            distribute = true;
            break;
          }
          otherObjects++;
        }
      }
    }
    i++;
  }
}
```

Kolizie pomocou bounding boxov
``` c++
if(obj->canCollide)
    {
      auto otherObjects = std::begin(objects);
      int collisionCounter = 0;
      while (otherObjects != std::end(objects)) 
      {
        auto oObj = otherObjects->get();
        collisionCounter = 0;
        if ((oObj != obj) && (oObj->canCollide == false))
        {
          if(fabs(obj->position.x - oObj->position.x) < ((oObj->size.x) + (obj->size.x))) //over koliziu
          {
            collisionCounter++;
          }
          if(fabs(obj->position.y - oObj->position.y) < ((oObj->size.y) + (obj->size.y)))
          {
            collisionCounter++;
          }
          if(fabs(obj->position.z - oObj->position.z) < ((oObj->size.z) + (obj->size.z)))
          {
            collisionCounter++;
          }
          if (collisionCounter == 3)
          {
            obj->fallSpeed *= 0.2; //1 pre aktivnejsiu sim, 0.2 pre splash
            if (oObj->normal.x != 0)
            {
              obj->position.x -= obj->speed.x * oObj->normal.x;
            }
            if (oObj->normal.y != 0)
            {
              obj->position.y -= obj->speed.y * oObj->normal.y;
            }
            if (oObj->normal.z != 0)
            {
              obj->position.z -= obj->speed.z * oObj->normal.z;
            }
          }
        }
        otherObjects++;
      }
    }
  }
```


Kolizie vodnych guliciek
``` c++
void Water::collide(Scene &scene) {

    for (auto& object : scene.objects){

        if (dynamic_cast<Water*>(object.get()) == nullptr)
            continue;

        if (abs(position.x - object->position.x) > scale.x*2 || abs(position.y - object->position.y) > scale.x*2 || abs(position.z - object->position.z) > scale.x*2 )
            continue;

        glm::vec3 pdif = position - object->position;
        float dis = glm::length(pdif);
        float col = dis - scale.x - object->scale.x;

        if (col < 0 && dis != 0) {
            auto ratio = -col / dis;
            auto vec = pdif * ratio;
            position += vec;
        }

    }
}
```

