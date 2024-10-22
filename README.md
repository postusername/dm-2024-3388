# Система компьютерной алгебры на коллоквиум по ДМиТИ


Система представляет собой Python-образный интерпретатор команд для работы с длинными числами. Код распространяется по лицензии AGPL-3.0.

[Справка по командам](./docs/help.md).

Для запуска (на Linux:

```bash
sudo apt-get install g++ make
git clone https://github.com/postusername/dm-2024-3388.git
cd dm-2024-3388
make algebra
cd build
./algebra
```

Для запуска тестов в той же директории:

```bash
make tests
cd build
./tests
```
