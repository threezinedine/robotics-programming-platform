from .application_builder import ApplicationBuilder


class EnvironmentBuilder:
    def __init__(self) -> None:
        self.applicationBuilder = ApplicationBuilder()

    def AddApplication(self, appBuilder: ApplicationBuilder) -> "EnvironmentBuilder":
        """
        Specify custom application builder to be used in the environment build.
        """
        self.applicationBuilder = appBuilder
        return self

    def EmptyApplication(self) -> "EnvironmentBuilder":
        """
        Specify that the appdata folder should not be existed. Only used in some tests.
        """
        self.applicationBuilder = None
        return self

    def Build(self) -> None:
        """
        Only actually run build of all needed environment here. Other defined via methods
        will be delegated to the methods.
        """
        if self.applicationBuilder is not None:
            self.applicationBuilder.Build()
