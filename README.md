# substring_searcher
find a substring in very long string.

Pointer 클래스는 2차원 배열에서 원소의 위치를 가리킨다.


Fasta 클래스는 fasta파일을 읽어 본문을 vector<char>형식으로 저장한다.
다른 클래스(LocalAlignment 등)에서 이용하기위해 getDNA함수를 이용해 dna 문자열에 접근할 수 있다.


CommonSequence 클래스는 local common sequence의 정보를 나타낸다. align matrix내의 시작점, 종료점, 점수를 나타낸다.


LocalAlignment 클래스는 두 문자열(vector<char>)를 입력받아 둘을 비교하는 lcs alignment matrix를 작성한다.

멤버 lcs는 점수가 가장 높은 상위 6개의 local common sequence의 정보를 저장한다.

align()은 alignment matrix를 작성한다.
작성 도중 match판정시 해당 지점을 종료점으로 하는 local common sequence를 찾아(traceOrigin()) lcs를 갱신한다.(registerLcs())

traceOrigin()은 인자로 받은 matrix상의 위치를 common sequence의 종료점으로 삼아 common sequence의 시작점을 찾아간다.
lcs의 원소 중 traceOrigin()이 찾는 common sequence의 substring이 존재하면 이를 삭제한다.

registerOrigin()은 lcs를 갱신한다. lcs의 원소 중 점수가 높은 상위 6개를 제외하고 나머지 원소는 모두 제거한다.
lcs의 멤버 중 하나는 traceOrigin()에 의해 삭제될 가능성이 있기에 점수가 가장 높은 상위 6개의 common sequence를 저장한다.

traceOrigin()과 getLCSVector()는 common sequence의 종료점을 기준으로 시작점을 찾아간다.
현 지점 (i, j)를 기준으로 alignMatrix[i-1][j-1] + <match point> == alignMatrix[i][j]일 경우 match로 간주하여 진행한다.
match로 간주할 수 없을 경우 alignMatrix[i-1][j], alignMatrix[i-1][j-1], alignMatrix[i][j-1]중 가장 값이 큰 곳을 향해 진행한다.
각각을 string2의 gap, missmatch, string1의 gap으로 간주한다.
