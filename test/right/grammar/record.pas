program RecordExample;

type
    Student = record
        name: string;
        age: integer;
        grade: real;
    end;

var
    student1: Student;

begin
    student1.name := 'John Doe';
    student1.age := 20;
    student1.grade := 85.5;

    writeln('Student Name: ', student1.name);
    writeln('Student Age: ', student1.age);
    writeln('Student Grade: ', student1.grade);
end.