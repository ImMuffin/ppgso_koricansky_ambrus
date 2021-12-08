# Projekt PPGSO

Autori: Adam Koričanský a Ondrej Ambruš

Team: Team 11

Téma: Akvárium

---
## Popis návrhu

## Implementacia

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

Pohyb kamery pripojeny na rybu. CameraFocus urcuje ktory objekt sledujeme.
``` c++
if (obj->cameraFocus == true)
{
    camera->back = obj->position;
    camera->position = obj->position + obj->forward * 2.0f;
}
```