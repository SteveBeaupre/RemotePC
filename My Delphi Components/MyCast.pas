unit MyCast;
interface
USES Windows,SysUtils,classes,Graphics;
////////////////////////////////////////////////////////////////////////////////////////////
CONST
 clOrange = $000080FF;
////////////////////////////////////////////////////////////////////////////////////////////
TYPE
 PINTEGER = ^INTEGER;
 PBYTE = ^BYTE;
 PSTRING = ^STRING;
 TSTRING_COMPARER_MODE = (mdBEGINING,mdENDING,mdANYWHERE,mdWHOLEWORD); //FOR STRING_COMPARER ONLY
 TLEFT_OR_RIGHT = (lrLeft,lrRight);
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION  PCHAR_TO_STRING(Buffer: Pointer; MaxBufferSize: Integer = 0): String;
PROCEDURE STRING_TO_PCHAR(s: String; Buffer: Pointer; BufferSize: Integer);
PROCEDURE NUMBER_INCREMENTER(Number: PINTEGER; ToAdd,LimitDown,LimitUp: INTEGER);
PROCEDURE CAPITALIZE(S: PSTRING);
FUNCTION  SAVESTRTOFILE(STRINGVAL,DEST: STRING): BOOLEAN;
FUNCTION  STRING_COMPARER(SUBSTR,STR: STRING; CASE_CHECK: BOOLEAN; MODE: TSTRING_COMPARER_MODE): BOOLEAN;
FUNCTION  PERCENTAGE(NUMBER,TOTAL: CURRENCY): CURRENCY;
FUNCTION  BOOLTOSTR(BOOLVAL: BOOLEAN): STRING;
FUNCTION  STRTOBOOL(STRINGVAL: STRING): BOOLEAN;
FUNCTION  GET_SUBSTR_OF_STRING(B,E: INTEGER; S: STRING): STRING;
FUNCTION  DAT_READER(LEFT_OR_RIGHT: TLEFT_OR_RIGHT; S: STRING): STRING;
FUNCTION  IS_IN_SQUARE(X1,X2,Y1,Y2,X,Y: INTEGER): BOOLEAN;
FUNCTION  INTTOBOOL(I: INTEGER): BOOLEAN;
FUNCTION  BOOLTOINT(B: BOOLEAN): INTEGER;
FUNCTION  DRIVE_BUG_CORRECTOR(S: STRING): STRING;
FUNCTION  REMOVEFILEEXT(FNAME: STRING): STRING;
Function  LongToShortPath(s: String): String;
////////////////////////////////////////////////////////////////////////////////////////////
implementation
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////CONVERT A C STRING INTO A DELPHI ONE////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION PCHAR_TO_STRING(Buffer: Pointer; MaxBufferSize: Integer = 0): String;
var
  Cpt: Integer;
  s: String;
  sz: PCHAR;
begin
sz := Buffer;
Cpt := 0;
s := '';
while(Ord(sz[Cpt]) <> 0) do begin
  s := s + sz[Cpt];
  Inc(Cpt);
  if(Cpt = MaxBufferSize) then
    break;
end;
Result := s;
end;
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////CONVERT A DELPHI STRING TO A C STRING///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
PROCEDURE STRING_TO_PCHAR(s: String; Buffer: Pointer; BufferSize: Integer);
var
  Cpt, Len: Integer;
  c: Char;
  pBuffer: PCHAR;
begin
Len := Length(s);

pBuffer := Buffer;
ZeroMemory(@pBuffer[0], BufferSize);
for Cpt := 1 to Len do begin
  if(Cpt = BufferSize) then
    break;
  c := s[Cpt];
  CopyMemory(@pBuffer[Cpt-1], @c, 1);
end;
end;
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////CHECK IF A STRING IS IN ANOTHER ONE////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION STRING_COMPARER(SUBSTR,STR: STRING; CASE_CHECK: BOOLEAN; MODE: TSTRING_COMPARER_MODE): BOOLEAN;
VAR
 CPT,CPT_MEM: INTEGER;
BEGIN
//INIT. THE FUNCTION
RESULT := FALSE;
//EXIT IF THE RESULT IS LOGIC...
IF (LENGTH(SUBSTR) > LENGTH(STR)) OR ((LENGTH(SUBSTR) = 0) AND (LENGTH(STR) > 0)) OR ((LENGTH(STR) = 0) AND (LENGTH(SUBSTR) > 0)) THEN BEGIN
 RESULT := FALSE;
 EXIT;
END;
IF ((LENGTH(SUBSTR) = 0) AND (LENGTH(STR) = 0)) THEN BEGIN
 RESULT := TRUE;
 EXIT;
END;
//CONVERT ALL TO UPPERCASE IF CASE_CHECK IS SET TO TRUE
IF CASE_CHECK THEN BEGIN
 STR := UPPERCASE(STR);
 SUBSTR := UPPERCASE(SUBSTR);
END;
//DIFFERRENT COMPARING MODE...
CASE MODE OF
 mdBEGINING: BEGIN
            //SET THE RESULT TO TRUE...
            RESULT := TRUE;
            //...AND SET IT TO FALSE BEFORE GOING OUT OF THE LOOP IF THE CHAR WEREN'T THE SAME
            FOR CPT := 1 TO LENGTH(SUBSTR) DO BEGIN
             RESULT := SUBSTR[CPT] = STR[CPT];
             IF RESULT = FALSE THEN BREAK;
            END;
           END;
 mdENDING: BEGIN
            //SET THE RESULT TO TRUE...
            RESULT := TRUE;
            //...AND SET IT TO FALSE BEFORE GOING OUT OF THE LOOP IF THE CHAR WEREN'T THE SAME
          FOR CPT := LENGTH(SUBSTR) DOWNTO 1 DO BEGIN
           RESULT := SUBSTR[CPT] = STR[CPT + (LENGTH(STR) - LENGTH(SUBSTR))];
           IF RESULT = FALSE THEN BREAK;
          END;
         END;
 mdANYWHERE,mdWHOLEWORD:
        BEGIN
         //SET THE RESULT TO FALSE...
         RESULT := FALSE;
         //CHECK FOR LOGICAL RESULT
         IF (MODE = mdWHOLEWORD) AND (LENGTH(SUBSTR) < LENGTH(STR)) THEN EXIT;
         //INIT. CPT_MEM
         CPT_MEM := 1;
         //CHECK ALL CHAR OF STR AND BREAK IF THE WORD IS FOUND
         FOR CPT := 1 TO LENGTH(STR) DO BEGIN
          IF SUBSTR[CPT_MEM] = STR[CPT] THEN BEGIN
           IF CPT_MEM = LENGTH(SUBSTR) THEN BEGIN
            RESULT := TRUE;
            BREAK;
           END;
           INC(CPT_MEM);
          END ELSE BEGIN
           IF MODE = mdWHOLEWORD THEN BEGIN
            RESULT := FALSE;
            BREAK;
           END;
           CPT_MEM := 1;
          END;
         END;
        END;
END;
END;
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////CAPITALISE A GIVEN STRING///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
PROCEDURE CAPITALIZE(S: PSTRING);
VAR
 CPT: INTEGER;
BEGIN
CPT := 1;
WHILE CPT < LENGTH(S^) DO BEGIN
 IF (ORD(S^[CPT]) IN [97..122]) AND ((CPT = 1) OR (S^[PRED(CPT)] = ' ')) THEN S^[CPT] := CHR(ORD(S^[CPT]) - 32);
 IF (ORD(S^[CPT]) IN [65..90]) AND (CPT > 1) AND (S^[PRED(CPT)] <> ' ') THEN S^[CPT] := CHR(ORD(S^[CPT]) + 32);
 INC(CPT);
END;
END;
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////INCREMENT A NUMBER AND CHECK IF IT HOLD IN THE LIMIT////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
PROCEDURE NUMBER_INCREMENTER(Number: PINTEGER; ToAdd,LimitDown,LimitUp: INTEGER);
BEGIN
INC(Number^,ToAdd);
IF Number^ IN [LimitDown..LimitUp] THEN BEGIN
 EXIT;
END ELSE BEGIN
 IF Number^ > LimitUp THEN BEGIN
  DEC(Number^,LimitUp);
 END ELSE BEGIN
  Number^ := LimitUp - Number^;
 END;
END;
END;
////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////FIND THE PERCENTAGE OF A NUMBER AND THE TOTAL////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION PERCENTAGE(NUMBER,TOTAL: CURRENCY): CURRENCY;
BEGIN
RESULT := ((NUMBER * 100) / TOTAL);
END;
////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////RETURN THE STRING EQUIVALENT OF A BOOEAN VALUE///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION BOOLTOSTR(BOOLVAL: BOOLEAN): STRING;
BEGIN
CASE BOOLVAL OF
 FALSE: RESULT := 'FALSE';
 ELSE RESULT := 'TRUE';
END;
END;
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////RETURN THE BOOL EQUIVALENT OF A STRING VALUE////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION STRTOBOOL(STRINGVAL: STRING): BOOLEAN;
BEGIN
IF UPPERCASE(STRINGVAL) = 'FALSE' THEN RESULT := FALSE ELSE RESULT := TRUE;
END;
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////SAVE A STRING TO THE DISK DESTINATION///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION SAVESTRTOFILE(STRINGVAL,DEST: STRING): BOOLEAN;
VAR
 STRLST: TSTRINGLIST;
BEGIN
RESULT := FALSE;
STRLST := TSTRINGLIST.CREATE;
STRLST.TEXT := STRINGVAL;
TRY
 STRLST.SAVETOFILE(DEST);
FINALLY
 STRLST.FREE;
END;
END;
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////RETURN A SUBSTRING OF A STRING FROM B TO E/////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION GET_SUBSTR_OF_STRING(B,E: INTEGER; S: STRING): STRING;
VAR
 CPT: INTEGER;
 RES: STRING;
BEGIN
RES := '';
CPT := B;
WHILE CPT <= E DO BEGIN RES := RES + S[CPT]; INC(CPT); END;
RESULT := RES;
END;
////////////////////////////////////////////////////////////////////////////////////////////
/////////USE THIS FUNCTION TO READ ONE OF THE TWO DATA IN A STRING LIKE ABC = DEF///////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION DAT_READER(LEFT_OR_RIGHT: TLEFT_OR_RIGHT; S: STRING): STRING;
VAR
 BEG,FIN: INTEGER;
BEGIN
IF LENGTH(S) > 0 THEN
 IF LEFT_OR_RIGHT = lrLeft THEN BEGIN
  FIN := LENGTH(S);
  BEG := 1;
  WHILE S[BEG] <> ' ' DO INC(BEG);
  DELETE(S,BEG,FIN);
 END ELSE BEGIN
  FIN := LENGTH(S);
  BEG := 1;
  WHILE S[FIN] <> ' ' DO
   DEC(FIN);
  DELETE(S,BEG,FIN);
 END;
RESULT := S;
END;
////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////THIS FUNC TELL IF A POINT IS IN A SQUARE//////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION IS_IN_SQUARE(X1,X2,Y1,Y2,X,Y: INTEGER): BOOLEAN;
BEGIN
RESULT := (((X >= X1) AND (X <= X2)) AND ((Y >= Y1) AND (Y <= Y2)));
END;
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////CONVERT A NUMBER TO A BOOL VALUE///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION INTTOBOOL(I: INTEGER): BOOLEAN;
BEGIN
CASE I OF
 0: RESULT := FALSE;
 ELSE RESULT := TRUE;
END;
END;
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////CONVERT A BOOL TO A NUMERIC VALUE//////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION BOOLTOINT(B: BOOLEAN): INTEGER;
BEGIN
CASE B OF
 false: RESULT := 0;
 else   RESULT := 1;
END;
END;
////////////////////////////////////////////////////////////////////////////////////////////
//////////CORRECT A LITTLE FUCKING BUG(EX: APPDIR = 'C:\' + '\NOM DE REPERTOIRE)////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION DRIVE_BUG_CORRECTOR(S: STRING): STRING;
BEGIN
{$B-}
IF (LENGTH(S) = 3) AND (S[2] = ':') AND (S[3] = '\') AND (ORD(S[1]) IN [65..90,97..122]) THEN
 DELETE(S,3,1);
RESULT := S;
END;
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////REMOVE THE EXTENSION OF A FILE NAME////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
FUNCTION REMOVEFILEEXT(FNAME: STRING): STRING;
var
  FileExt: String;
BEGIN
FileExt := ExtractFileExt(FNAME);
DELETE(FNAME,(LENGTH(FNAME)+1)-LENGTH(FileExt),LENGTH(FileExt));
RESULT := FNAME;
END;
////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////GET THE SHORT PAHT OF A LONG PATH NAME///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
Function LongToShortPath(S: STRING): STRING;
var
 LPath, SPath: ARRAY [0..255] OF CHAR;
 Cpt: Integer;
BEGIN
FOR Cpt := 1 TO length(s) DO
  LPath[Cpt-1] := s[Cpt];
LPath[length(s)] := #0;
IF GetShortPathName(LPath, SPath , 256) > 0 THEN BEGIN
  Result := SPath;
END ELSE BEGIN
  Result := 'Path not exist!';
END;
END;
//////////////////////////////////////EOF///////////////////////////////////////////////////
end.
