# Что в репозитории
Репозиторий содержит решение 15 варианта первого практикума по формалкам. На вход подаётся регулярное выражение в обратной польской записи и строка s. Нужно вывести длину длину самого длинного префикса s, являющемся префиксом какого-либо слова из языка, задаваемого регулярным выражением.

# Использование

1) mkdir build
2) cd ./build
3) cmake .. && make main
4) ./main

# Тестирование 

Авто-тестирование реализовано с помощью **gtest**. Запустить можно с помощью
1) mkdir build
2) cd ./build
3) cmake .. && make tests
4) ./tests
5) lcov -t "tests" -o tests.info -c -d .  
6) genhtml -o report tests.info
7) cd ./report
8) В текущей папке будет файл index.html, содержащий процент покрытия кода тестами.
