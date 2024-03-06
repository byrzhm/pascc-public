program RecordParamExample;
type
    Student = record
        name: string;
        age: integer;
        grade: real;
    end;

procedure PrintStudentInfo(s: Student);
begin
    writeln('Student Name: ', s.name);
    writeln('Student Age: ', s.age);
    writeln('Student Grade: ', s.grade);
end;

var
    student1: Student;

begin
    student1.name := 'John Doe';
    student1.age := 20;
    student1.grade := 85.5;

    PrintStudentInfo(student1);
end.