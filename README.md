[![License](https://img.shields.io/badge/License-BSD%202--Clause-orange.svg)](https://opensource.org/licenses/BSD-2-Clause)
[![CI](https://github.com/leejeonghun/san11pk-kr/actions/workflows/main.yml/badge.svg)](https://github.com/leejeonghun/san11pk-kr/actions/workflows/main.yml)
# san11pk-kr


코에이테크모의 스팀판 [삼국지11 with 파워업키트](https://store.steampowered.com/app/628070/Romance_of_the_Three_Kingdoms_XI_with_Power_Up_Kit)의 한글 패치입니다.


## 기능

본 패치 적용시, 패키지 버전용 한글패치의 실행 파일 내 번역된 문자열을 스팀판 실행 파일에 적용시켜 줍니다. 번역된 문자열은 [JSON 파일](https://github.com/leejeonghun/san11pk-kr/tree/master/translate/san11wpk.exe.json) 포맷으로 관리되며 패키지 버전용 한글패치 대비 아래와 같은 추가적인 기능이 있습니다.
* 스팀판 실행파일로 구동되기 때문에 스팀 클라우드를 사용 가능합니다.
* 제약이 있지만, 신무장 작성 등의 문자 입력 시 한글 입력이 가능합니다.
  * 폰트에 존재하지 않는 글자는 정상 출력되지 않습니다.
  * 자음 폰트의 부재로 인해 초성 입력 단계에서 글자가 정상적으로 출력되지 않습니다. 중성 및 종성을 이어서 타이핑 하면 정상 출력됩니다.


## 사용법

* [삼국지11 with 파워업키트](https://store.steampowered.com/app/628070/Romance_of_the_Three_Kingdoms_XI_with_Power_Up_Kit)를 설치 후 언어를 일본어로 변경합니다.
* [여기](https://cafe.naver.com/sam10/601725)에서 패키지 버전용 한글패치를 다운로드 받아 스팀판 설치 경로에 압축 해제 및 덮어쓰기 합니다.
* [Releases](https://github.com/leejeonghun/san11pk-kr/releases) 항목에서 본 패치를 다운로드 받아 압축파일 내 d3d9.dll 파일을 게임 설치 폴더(실행 파일인 San11WPK.exe 파일이 있는 폴더)에 복사한 후 게임을 실행하면 패키지 버전용 한글패치가 스팀판에 적용되어 실행됩니다.
* 개인적으로 만든 프로그램이라, 디지털 서명이 되어 있지 않습니다. 다운로드 및 압축 해제 시 웹브라우저 보안 경고 및 윈도우에서 스마트 스크린 경고가 뜰 수 있으므로 참고 부탁드립니다.


## 라이선스

BSD 라이선스를 따릅니다. MIT 라이선스인 [JSON for Modern C++](https://github.com/nlohmann/json)을 사용합니다.
