from .application_builder import ApplicationBuilder
from .project_builder import ProjectBuilder


class EnvironmentBuilder:
    def __init__(self) -> None:
        self.applicationBuilder = ApplicationBuilder()
        self.projectBuilders: list[ProjectBuilder] = []

    def AddApplication(self, appBuilder: ApplicationBuilder) -> "EnvironmentBuilder":
        """
        Specify custom application builder to be used in the environment build.
        """
        self.applicationBuilder = appBuilder
        return self

    def AddProject(self, projectBuilder: ProjectBuilder) -> "EnvironmentBuilder":
        """
        Add a project to be created in the environment.
        """
        self.projectBuilders.append(projectBuilder)
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

        for projectBuilder in self.projectBuilders:
            projectBuilder.Build()
