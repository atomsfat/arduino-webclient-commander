#### Arduino webclient commander
Obtener los submodulos
```
git submodule init
```

Y crear el siguiente alias en la raiz del proyecto
```
ln -s arduino_submodules/Arduino-EasyTransfer/EasyTransfer libraries/EasyTransfer
```
es necesario instalar las dependecias node _express_
```
npm install  
```
Para correr el servidor _node_
```
node app.js
```

Al skecth de arduino WebClient es necesario configurarle la IP.



