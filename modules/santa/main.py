import random
from textual import events
from textual.app import App, ComposeResult
from textual.binding import Binding
from textual.widgets import Footer, Static

PARTICIPANTS = {
    "Sean": ["Catherine"],
    "Catherine": ["Sean"],
    "Margaret": ["Chris"],
    "Chris": ["Margaret"],
    "Arlene": ["Peter"],
    "Peter": ["Arlene"],
}


def violates_exclusions(matches):
    for giver, reciever in matches.items():
        if reciever in PARTICIPANTS[giver]:
            return True
    return False


def draw(participants):
    if len(participants) < 2:
        print("Not enough participants for a Secret Santa draw.")
        return {}

    givers = list(participants)
    receivers = list(participants)
    matches = {}

    random.shuffle(givers)
    random.shuffle(receivers)

    for giver in givers:
        # Find a suitable receiver
        potential_receivers = [r for r in receivers if r != giver]
        if not potential_receivers:
            # If no suitable receiver found, reshuffle and try again
            return draw(participants)

        receiver = random.choice(potential_receivers)
        matches[giver] = receiver
        receivers.remove(receiver)

    if violates_exclusions(matches):
        return draw(participants)

    return matches


class SantaApp(App):
    CSS = """
    Screen { align: center middle; }
    Static { width: auto; }
    """

    state_screen = 0
    screens = []

    BINDINGS = [
        Binding(key="space", action="advance", description="(Press space to advance)")
    ]

    def progress(self) -> None:
        if self.state_screen < len(self.screens):
            self.query_one(Static).update(self.screens[self.state_screen])
            self.state_screen += 1

    def make_screens(self):
        matches = draw(PARTICIPANTS)
        self.screens.append("Get ready for the drawing!")
        for giver, reciever in matches.items():
            self.screens.append(f"Pass the screen to {giver}.")
            self.screens.append(f"{giver}, you are giving to {reciever}!")
        self.screens.append("")

    ## CALLBACKS

    def compose(self) -> ComposeResult:
        yield Static("")
        yield Footer()

    def on_ready(self) -> None:
        self.make_screens()
        self.progress()

    def on_key(self, event: events.Key) -> None:
        self.progress()




if __name__ == "__main__":
    app = SantaApp()
    app.run()
