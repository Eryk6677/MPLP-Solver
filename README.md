# MPLP Solver:

## 1. Opis:
Program to prosty launcher, umożliwiający intuicyjne wprowadzanie instancji problemu MPLP, które mają być rozwiązywane przy pomocy zaimplementowanych algorytmów.
Dostępne są dwa sposoby podawania instancji.
Pierwszym z nich jest plik tekstowy, zwierający instancje, które mają być rozwiązane, struktura pliku będzie opisana w części "Plik wejściowy".
Drugim sposobem jest wprowadzenie jednej instancji przy pomocy czterech pól numerycznych opisanych odpowiednio długościami i szerokościami palety i elementu.

## 2. Struktura programu:
Pierwszym elementem otwartego programu jest menu typu "drop-down" umożliwiające wybór interesującego nas algorytmu.
Kolejnym elementem jest pole tekstowe "File path", umożliwia ono wprowadzenie ścieżki do pliku lub wybranie pliku przy pomocy eksploratora.
Kolejna część interfejsu to cztery pola numeryczne, odpowiednio przyjmujące wartości: długość palety, szerokość palety, długość elementu oraz szerokość elementu.
Należy zaznaczyć, że pole tekstowe ma priorytet.
Oznacza to, że jeśli nie jest ono puste, jego wartość zostanie potraktowana jako ścieżka do pliku a pola numeryczne do wprowadzania instancji zostaną zablokowane.
Ostatnie dwa pola numeryczne znajdujące się niżej to dwa parametry wykonania: maksymalna głębokość rekurencji oraz maksymalny czas obliczeń podawany w minutach.
Na samym dole okna po kliknięciu przycisku "Solve" wyświetlone zostaną otrzymane wyniki.

## 3. Zaimplementowane algorytmy:
 - Five Block Heuristic - Rekurencyjna heurystyka pięciu bloków,
 - Modified Five Block Heuristic - Zmodyfikowana rekurencyjna heurystyka pięciu bloków,
 - Improved Five Block Heuristic - Usprawniona rekurencyjna heurystyka pięciu bloków z zapamiętywaniem rozwiązań pośrednich.

## 4. Plik wejściowy:
Chcąc rozwiązać kilka instancji przy jednym wywołaniu, można skorzystać z pliku tekstowego.
Instancje muszą być w nim zapisane w konkretny sposób, każda instancja zawarta jest w kolejnej linii pliku.
Parametry tych instancji są podane w następującej kolejności: długość palety, szerokość palety, długość elementu, szerokość elementu.
Parametry instancji muszą być oddzielone znakami białymi, np. tabulacjami. 

### Przykład:
``15	11	3	2``\
``15	13	3	2``\
``15	15	3	2``\
``17	10	3	2``\
``17	12	3	2``\
``17	14	3	2``\
``17	16	3	2``

## 5. Pliki wyjściowe:
Dla każdej rozwiązywanej instancji, generowany jest plik wyjściowy o nazwie: długośćPalety_szerokośćPalety_długośćElementu_szerokośćElementu.txt.
W pliku znajduje się lista rozmieszczonych na powierzchni palety elementów.
Każdy element jest zapisany w postaci dwóch cyfr (współrzędnych lewego dolnego rogu elementu) oraz jednej litery (H lub V, określających orientację elementu).
Tak przygotowane pliki można dostarczyć na wejście skryptu drawing.py opisanego niżej, którego rezultatem będzie czytelna reprezentacja graficzna rozwiązania.
Wszystkie pliki wynikowe zapisywane są w folderze "results" tworzonym w miejscu, gdzie znajduje się plik wykonywalny.

Jeśli wykorzystywana jest funkcjonalność dostarczenia pliku wejściowego zawierającego wiele instancji, wyniki ich rozwiązania zostaną zapisane w pliku "summary.csv".
Plik ten zostanie wygenerowany po ukończeniu rozwiązywania wszystkich podanych na wejściu instancji.
Zawiera on oddzielone przecinkami wartości wynikowe dla każdej instancji.
Struktura każdej linii tego pliku wynikowego wygląda następująco:
długość Palety, szerokość Palety, długość elementu, szerokość elementu, oszacowanie górne, otrzymane rozwiązanie, czas rozwiązania 

# DRAWING.PY

## 1. Opis
Skrypt "drawing.py" to niezależny moduł zaimplementowany w języku Python z wykorzystaniem biblioteki PyGame.
Zadaniem tego skryptu jest wygenerowanie pliku graficznego z otrzymanym ułożeniem elementów na powierzchni palety.
Wejściem skryptu jest plik tekstowy zawierający wyznaczoną wcześniej listę punktów i orientacji elementów (wyjście programu Launcher).
Finalnie, by zapewnić czytelność grafiki, paleta zostaje przedstawiona w postaci szarego prostokąta z czarnymi ramkami.
Elementy reprezentowane są przez białe, obramowane na czarno prostokąty.

## 2. Uruchomienie
By skorzystać z generatora, należy pobrać interpreter języka Python3!
Następnie wykonać poniższą komendę: "python3 <ścieżka do pliku drawing.py> <ścieżka do pliku z punktami długośćPalety_szerokośćPalety_długośćElementu_szerokośćElementu.txt>"
