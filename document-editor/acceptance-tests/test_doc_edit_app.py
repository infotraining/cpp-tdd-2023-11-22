import pytest
import subprocess

app_path = './build/bin/document-editor.exe'


def read_from_console(proc: subprocess.Popen):
    return proc.stdout.read1().decode('utf-8')


def write_to_console(proc: subprocess.Popen, text: bytes):
    proc.stdin.write(text)
    proc.stdin.flush()


def test_exit_command_exits_application():
    try:
        proc = subprocess.Popen([app_path],
                                stdout=subprocess.PIPE, stdin=subprocess.PIPE)
        prompt = read_from_console(proc)
        assert prompt == '> Enter a command: \r\n'

        write_to_console(proc, b'Exit\r\n')
        proc.wait()
        assert proc.returncode == 0
    except AssertionError as e:
        proc.kill()
        proc.wait()
        raise e


@pytest.fixture
def running_app():
    try:
        proc = subprocess.Popen(['./bin/doc-edit.exe'],
                                stdout=subprocess.PIPE, stdin=subprocess.PIPE)
        prompt = read_from_console(proc)
        assert prompt == '> Enter a command: \r\n'

        yield proc

        write_to_console(proc, b'Exit\r\n')
        proc.wait(1)
        assert proc.returncode == 0
    except Exception as e:
        proc.kill()
        proc.wait()
        raise e


@pytest.fixture
def doc_with_text(running_app):
    write_to_console(running_app, b'AddText\r\n')
    output = read_from_console(running_app)
    assert output == 'Write text: \r\n'
    write_to_console(running_app, b'Hello World!\r\n')
    output = read_from_console(running_app)
    assert output == 'Enter a command: \r\n'
    return running_app


def test_print_command_on_empty_doc(running_app):
    write_to_console(running_app, b'Print\r\n')
    output = read_from_console(running_app)
    assert output == '[]\r\n'


def test_add_text_command_for_empty_document(running_app):
    write_to_console(running_app, b'AddText\r\n')
    output = read_from_console(running_app)
    assert output == 'Write text: \r\n'
    write_to_console(running_app, b'Hello World!\r\n')
    output = read_from_console(running_app)
    assert output == 'Enter a command: \r\n'
    write_to_console(running_app, b'Print\r\n')
    output = read_from_console(running_app)
    assert output == '[Hello World!]\r\n'


def test_add_text_command_adds_text_at_the_end_of_document(running_app, doc_with_text):
    write_to_console(running_app, b'AddText\r\n')
    output = read_from_console(running_app)
    assert output == 'Write text: \r\n'
    write_to_console(running_app, b'_NewText\r\n')
    output = read_from_console(running_app)
    assert output == 'Enter a command: \r\n'
    write_to_console(running_app, b'Print\r\n')
    output = read_from_console(running_app)
    assert output == '[Hello World!_NewText]\r\n'


def test_to_upper_command_converts_document_to_uppercase(running_app, doc_with_text):
    write_to_console(running_app, b'ToUpper\r\n')
    output = read_from_console(running_app)
    write_to_console(running_app, b'Print\r\n')
    output = read_from_console(running_app)
    assert output == '[HELLO WORLD!]\r\n'
